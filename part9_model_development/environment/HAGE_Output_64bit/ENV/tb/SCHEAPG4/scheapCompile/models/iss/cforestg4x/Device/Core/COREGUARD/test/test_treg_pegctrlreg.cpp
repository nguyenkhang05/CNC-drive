/**
 * @file test_treg_pegctrlreg.cpp
 *
 * @date Created on: 2016
 * @author Author: Quang Lam <quang.lam.px@rvc.renesas.com>
 * @brief UNIT TEST for PEG Control register. (SAMPLE)
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

class TRegPegCtrlReg;

// ----------------------------------------
class TRegPegCtrlReg1 : public testing::Test {
protected:
    TRegPegCtrlReg1() {}
    virtual ~TRegPegCtrlReg1() {}
    virtual void SetUp() {
        m_treg_pegctrlreg   = new TRegPegCtrlReg (base_pegctrlreg, CORE_G4MH, CORE_VER_20);
    }
    virtual void TearDown() { delete m_treg_pegctrlreg; }

public:
    TRegPegCtrlReg   *m_treg_pegctrlreg;
    MockSideBand sideband;

public:
    uint32_t base_pegctrlreg = 0xFFC6C000;
};


/*!
 * @brief Check Initial method.
 */

TEST_F(TRegPegCtrlReg1, Check_Initial_Setup) {
    ASSERT_NE(nullptr, m_treg_pegctrlreg->GetPegKceProt());
    ASSERT_EQ(base_pegctrlreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, m_treg_pegctrlreg->GetPegKceProt()->GetRegAddr());
    ASSERT_STREQ("PEGKCEPROT", m_treg_pegctrlreg->GetPegKceProt()->GetName());

    for (uint32_t chn = 0; chn < TRegPegCtrlReg::NUM_PEG_CTRL_CHANNEL_G4MH2X; chn++) {
        std::ostringstream pegprot_name;
        pegprot_name << "PEGPROT" << chn;
        ASSERT_NE(nullptr, m_treg_pegctrlreg->GetPegProt(chn));
        ASSERT_STREQ(pegprot_name.str().c_str(), m_treg_pegctrlreg->GetPegProt(chn)->GetName());
        ASSERT_EQ(base_pegctrlreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn, m_treg_pegctrlreg->GetPegProt(chn)->GetRegAddr());

        std::ostringstream pegspid_name;
        pegspid_name << "PEGSPID" << chn;
        ASSERT_NE(nullptr, m_treg_pegctrlreg->GetPegSpid(chn));
        ASSERT_STREQ(pegspid_name.str().c_str(), m_treg_pegctrlreg->GetPegSpid(chn)->GetName());
        ASSERT_EQ(base_pegctrlreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn, m_treg_pegctrlreg->GetPegSpid(chn)->GetRegAddr());

        std::ostringstream pegbad_name;
        pegbad_name << "PEGBAD" << chn;
        ASSERT_NE(nullptr, m_treg_pegctrlreg->GetPegBad(chn));
        ASSERT_STREQ(pegbad_name.str().c_str(), m_treg_pegctrlreg->GetPegBad(chn)->GetName());
        ASSERT_EQ(base_pegctrlreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn, m_treg_pegctrlreg->GetPegBad(chn)->GetRegAddr());

        std::ostringstream pegadv_name;
        pegadv_name << "PEGADV" << chn;
        ASSERT_NE(nullptr, m_treg_pegctrlreg->GetPegAdv(chn));
        ASSERT_STREQ(pegadv_name.str().c_str(), m_treg_pegctrlreg->GetPegAdv(chn)->GetName());
        ASSERT_EQ(base_pegctrlreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn, m_treg_pegctrlreg->GetPegAdv(chn)->GetRegAddr());
    }
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
