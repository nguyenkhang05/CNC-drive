/**
 * @file test_treg_crgsecurereg_20.cpp
 *
 * @date Created on: 2018
 * @author Author: Hoanh Ho <hoanh.ho.px@renesas.com>
 * @brief UNIT TEST for CRGSECUREREG
 * @par Copyright: 2018 Renesas Electronics Corp.
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

class TRegCrgSecureReg;

// ----------------------------------------
class TRegCrgSecureReg1 : public testing::Test {
protected:
    TRegCrgSecureReg1() {}
    virtual ~TRegCrgSecureReg1() {}
    virtual void SetUp() {
        GuardInternal * guard_internal= new GuardInternal(false, false,
                                                              0x13, 0x8, 0x8, 0x4, 0xA5A5A500, 0x0);
        m_treg_crgsecurereg   = new TRegCrgSecureReg (base_crgreg_ivc, CORE_G4MH, CORE_VER_20, guard_internal);
    }
    virtual void TearDown() { delete m_treg_crgsecurereg; }

public:
    TRegCrgSecureReg   *m_treg_crgsecurereg;
    MockSideBand sideband;

public:
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

TEST_F(TRegCrgSecureReg1, Check_Initial_Setup) {
    for (uint32_t chn = 0; chn < GuardInternal_Interface::NUM_CRG_SEC_CHANNEL; chn++) {
        std::ostringstream csgprot_name;
        csgprot_name << "CSGPROT" << chn;
        ASSERT_NE(nullptr, m_treg_crgsecurereg->GetCrgProt(chn));
        ASSERT_STREQ(csgprot_name.str().c_str(), m_treg_crgsecurereg->GetCrgProt(chn)->GetName());
        ASSERT_EQ(base_crgreg_ivc + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn, m_treg_crgsecurereg->GetCrgProt(chn)->GetRegAddr());

        std::ostringstream csgspid_name;
        csgspid_name << "CSGSPID" << chn;
        ASSERT_NE(nullptr, m_treg_crgsecurereg->GetCrgSpid(chn));
        ASSERT_STREQ(csgspid_name.str().c_str(), m_treg_crgsecurereg->GetCrgSpid(chn)->GetName());
        ASSERT_EQ(base_crgreg_ivc + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn, m_treg_crgsecurereg->GetCrgSpid(chn)->GetRegAddr());

        std::ostringstream csgbad_name;
        csgbad_name << "CSGBAD" << chn;
        ASSERT_NE(nullptr, m_treg_crgsecurereg->GetCrgBad(chn));
        ASSERT_STREQ(csgbad_name.str().c_str(), m_treg_crgsecurereg->GetCrgBad(chn)->GetName());
        ASSERT_EQ(base_crgreg_ivc + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn, m_treg_crgsecurereg->GetCrgBad(chn)->GetRegAddr());

        std::ostringstream csgadv_name;
        csgadv_name << "CSGADV" << chn;
        ASSERT_NE(nullptr, m_treg_crgsecurereg->GetCrgAdv(chn));
        ASSERT_STREQ(csgadv_name.str().c_str(), m_treg_crgsecurereg->GetCrgAdv(chn)->GetName());
        ASSERT_EQ(base_crgreg_ivc + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn, m_treg_crgsecurereg->GetCrgAdv(chn)->GetRegAddr());
    }
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
