/**
 * @file test_peg.cpp
 *
 * @date Created on: 2016
 * @author Author: Quang Lam <quang.lam.px@rvc.renesas.com>
 * @brief UNIT TEST for PEG. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include <memory>
#include <map>
#include <stdexcept>
#include "treg_guard.h"
#include "sideband.h"
#include "peg.h"
#include "mock_sideband.h"
#include "gmock/gmock.h"

// using ::testing::AtLeast;
// using ::testing::_;
// using ::testing::Return;

class Peg;

static const uint32_t PE0_PEG_UT = 0x0;
static const uint32_t PE1_PEG_UT = 0x1;

// ----------------------------------------
class Peg1 : public testing::Test {
protected:
    Peg1() {}
    virtual ~Peg1() {}
    virtual void SetUp() {
        m_peg           = new Peg (m_peid, base_pegreg, CORE_G4MH, CORE_VER_11);
    }
    virtual void TearDown() { delete m_peg; }

public:
    Peg          *m_peg;
    MockSideBand sideband;

public:
    uint32_t m_peid = PE0_PEG_UT;
    uint32_t base_pegreg = 0xFFC6C000;
    union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };
};


/*!
 * @brief Check Initial method.
 */

TEST_F(Peg1, Check_Initial_Setup) {
    ASSERT_NE(nullptr, m_peg->GetPegGuard());
    ASSERT_NE(nullptr, m_peg->GetPegCtrl());
}


/*!
 * @brief Check Reset method.
 */
TEST_F(Peg1, Check_Reset) {
    SideBand sb_info;
    // Change addr value in PEGADDR to confirm if it is reset or not.
    m_peg->GetPegCtrl()->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT);
    ASSERT_EQ(0x1U, m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegKceProt()->GetKCE());
    m_peg->HardReset();
    ASSERT_EQ(0x0U, m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegKceProt()->GetKCE());
}


/*!
 * @brief Check TargetWrite method.
 */
TEST_F(Peg1, Check_TargetWrite) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    ASSERT_THROW(m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);

    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT);

    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x53, 0x01, 0x00, 0x00}, SIZE_32BIT);
    // at least, 1 PEG channel setting is enable.

    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);

    ASSERT_EQ(0x1U, m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegKceProt()->GetKCE());
    ASSERT_EQ(0x1U, m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegProt(chn_0)->GetGEN());
    ASSERT_EQ(0xFFFFFFFFU, m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegSpid(chn_0)->GetSPID());
    ASSERT_EQ(0xFFFFF000U, m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegBad(chn_0)->GetBAD());
    ASSERT_EQ(0x03FFF000U, m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegAdv(chn_0)->GetADV());
}


/*!
 * @brief Check TargetRead method.
 */
TEST_F(Peg1, Check_TargetRead) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    uint8_t data_8_input_pegkcprot[4] = {0x01, 0xa5, 0xa5, 0xa5};
    uint8_t data_8_input_pegprot[4] = {0x53, 0x01, 0x00, 0x00};
    uint8_t data_8_input_pegspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_pegbad[4] = {0x00, 0xf0, 0xff, 0xff};
    uint8_t data_8_input_pegadv[4] = {0x00, 0xf0, 0xff, 0x03};

    uint8_t data_8_output_pegkcprot[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegprot[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegbad[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegadv[4]    = {0xff, 0xff, 0xff, 0xff};

    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, data_8_input_pegkcprot, SIZE_32BIT);
    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegprot, SIZE_32BIT);
    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegspid, SIZE_32BIT);
    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegbad, SIZE_32BIT);
    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegadv, SIZE_32BIT);

    m_peg->TargetRead(sb_info, base_pegreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, data_8_output_pegkcprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegkcprot, sizeof(m_data32));
    uint32_t m_data32_out_pegkcprot = m_data32;
    // Get KCE key in PEGKCPROT.
    memcpy (&m_data32, data_8_input_pegkcprot, sizeof(m_data32));
    uint32_t m_data32_in_pegkcprot = m_data32 & 0x01;
    ASSERT_EQ(m_data32_out_pegkcprot, m_data32_in_pegkcprot);

    m_peg->TargetRead(sb_info, base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegprot, sizeof(m_data32));
    uint32_t m_data32_out_pegprot = m_data32;
    memcpy (&m_data32, data_8_input_pegprot, sizeof(m_data32));
    uint32_t m_data32_in_pegprot = m_data32;
    ASSERT_EQ(m_data32_out_pegprot, m_data32_in_pegprot);

    m_peg->TargetRead(sb_info, base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegspid, sizeof(m_data32));
    uint32_t m_data32_out_pegspid = m_data32;
    memcpy (&m_data32, data_8_input_pegspid, sizeof(m_data32));
    uint32_t m_data32_in_pegspid = m_data32;
    ASSERT_EQ(m_data32_out_pegspid, m_data32_in_pegspid);

    m_peg->TargetRead(sb_info, base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegbad, sizeof(m_data32));
    uint32_t m_data32_out_pegbad = m_data32;
    memcpy (&m_data32, data_8_input_pegbad, sizeof(m_data32));
    uint32_t m_data32_in_pegbad = m_data32;
    ASSERT_EQ(m_data32_out_pegbad, m_data32_in_pegbad);

    m_peg->TargetRead(sb_info,  base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegadv, sizeof(m_data32));
    uint32_t m_data32_out_pegadv = m_data32;
    memcpy (&m_data32, data_8_input_pegadv, sizeof(m_data32));
    uint32_t m_data32_in_pegadv = m_data32;
    ASSERT_EQ(m_data32_out_pegadv, m_data32_in_pegadv);
}


/*!
 * @brief Check TargetFetch method.
 */
TEST_F(Peg1, Check_TargetFetch) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    uint8_t data_8_input_pegkcprot[4] = {0x01, 0xa5, 0xa5, 0xa5};
    uint8_t data_8_input_pegprot[4] = {0x53, 0x01, 0x00, 0x00};
    uint8_t data_8_input_pegspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_pegbad[4] = {0x00, 0xf0, 0xff, 0xff};
    uint8_t data_8_input_pegadv[4] = {0x00, 0xf0, 0xff, 0x03};

    uint8_t data_8_output_pegkcprot[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegprot[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegbad[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegadv[4]    = {0xff, 0xff, 0xff, 0xff};

    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, data_8_input_pegkcprot, SIZE_32BIT);
    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegprot, SIZE_32BIT);
    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegspid, SIZE_32BIT);
    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegbad, SIZE_32BIT);
    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegadv, SIZE_32BIT);

    m_peg->TargetFetch(sb_info, base_pegreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, data_8_output_pegkcprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegkcprot, sizeof(m_data32));
    uint32_t m_data32_out_pegkcprot = m_data32;
    // Get KCE key in PEGKCPROT.
    memcpy (&m_data32, data_8_input_pegkcprot, sizeof(m_data32));
    uint32_t m_data32_in_pegkcprot = m_data32 & 0x01;;
    ASSERT_EQ(m_data32_out_pegkcprot, m_data32_in_pegkcprot);

    m_peg->TargetFetch(sb_info, base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegprot, sizeof(m_data32));
    uint32_t m_data32_out_pegprot = m_data32;
    memcpy (&m_data32, data_8_input_pegprot, sizeof(m_data32));
    uint32_t m_data32_in_pegprot = m_data32;
    ASSERT_EQ(m_data32_out_pegprot, m_data32_in_pegprot);

    m_peg->TargetFetch(sb_info, base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegspid, sizeof(m_data32));
    uint32_t m_data32_out_pegspid = m_data32;
    memcpy (&m_data32, data_8_input_pegspid, sizeof(m_data32));
    uint32_t m_data32_in_pegspid = m_data32;
    ASSERT_EQ(m_data32_out_pegspid, m_data32_in_pegspid);

    m_peg->TargetFetch(sb_info, base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegbad, sizeof(m_data32));
    uint32_t m_data32_out_pegbad = m_data32;
    memcpy (&m_data32, data_8_input_pegbad, sizeof(m_data32));
    uint32_t m_data32_in_pegbad = m_data32;
    ASSERT_EQ(m_data32_out_pegbad, m_data32_in_pegbad);

    m_peg->TargetFetch(sb_info,  base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegadv, sizeof(m_data32));
    uint32_t m_data32_out_pegadv = m_data32;
    memcpy (&m_data32, data_8_input_pegadv, sizeof(m_data32));
    uint32_t m_data32_in_pegadv = m_data32;
    ASSERT_EQ(m_data32_out_pegadv, m_data32_in_pegadv);
}


/*!
 * @brief Check TargetWriteDebug method.
 */
TEST_F(Peg1, Check_TargetWriteDebug) {
    uint32_t chn_0 = 0x0;

    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x53, 0x01, 0x00, 0x00}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);

    ASSERT_EQ(0x1U, m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegKceProt()->GetKCE());
    ASSERT_EQ(0x1U, m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegProt(chn_0)->GetGEN());
    ASSERT_EQ(0xFFFFFFFFU, m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegSpid(chn_0)->GetSPID());
    ASSERT_EQ(0xFFFFF000U, m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegBad(chn_0)->GetBAD());
    ASSERT_EQ(0x03FFF000U, m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegAdv(chn_0)->GetADV());
}


/*!
 * @brief Check TargetReadDebug method.
 */
TEST_F(Peg1, Check_TargetReadDebug) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    uint8_t data_8_input_pegkcprot[4] = {0x01, 0xa5, 0xa5, 0xa5};
    uint8_t data_8_input_pegprot[4] = {0x53, 0x01, 0x00, 0x00};
    uint8_t data_8_input_pegspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_pegbad[4] = {0x00, 0xf0, 0xff, 0xff};
    uint8_t data_8_input_pegadv[4] = {0x00, 0xf0, 0xff, 0x03};

    uint8_t data_8_output_pegkcprot[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegprot[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegbad[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegadv[4]    = {0xff, 0xff, 0xff, 0xff};

    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, data_8_input_pegkcprot, SIZE_32BIT);
    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegprot, SIZE_32BIT);
    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegspid, SIZE_32BIT);
    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegbad, SIZE_32BIT);
    m_peg->TargetWrite(sb_info, base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegadv, SIZE_32BIT);

    m_peg->TargetReadDebug(base_pegreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, data_8_output_pegkcprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegkcprot, sizeof(m_data32));
    uint32_t m_data32_out_pegkcprot = m_data32;
    // Get KCE key in PEGKCPROT.
    memcpy (&m_data32, data_8_input_pegkcprot, sizeof(m_data32));
    uint32_t m_data32_in_pegkcprot = m_data32 & 0x01;;
    ASSERT_EQ(m_data32_out_pegkcprot, m_data32_in_pegkcprot);

    m_peg->TargetReadDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegprot, sizeof(m_data32));
    uint32_t m_data32_out_pegprot = m_data32;
    memcpy (&m_data32, data_8_input_pegprot, sizeof(m_data32));
    uint32_t m_data32_in_pegprot = m_data32;
    ASSERT_EQ(m_data32_out_pegprot, m_data32_in_pegprot);

    m_peg->TargetReadDebug(base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegspid, sizeof(m_data32));
    uint32_t m_data32_out_pegspid = m_data32;
    memcpy (&m_data32, data_8_input_pegspid, sizeof(m_data32));
    uint32_t m_data32_in_pegspid = m_data32;
    ASSERT_EQ(m_data32_out_pegspid, m_data32_in_pegspid);

    m_peg->TargetReadDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegbad, sizeof(m_data32));
    uint32_t m_data32_out_pegbad = m_data32;
    memcpy (&m_data32, data_8_input_pegbad, sizeof(m_data32));
    uint32_t m_data32_in_pegbad = m_data32;
    ASSERT_EQ(m_data32_out_pegbad, m_data32_in_pegbad);

    m_peg->TargetReadDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegadv, sizeof(m_data32));
    uint32_t m_data32_out_pegadv = m_data32;
    memcpy (&m_data32, data_8_input_pegadv, sizeof(m_data32));
    uint32_t m_data32_in_pegadv = m_data32;
    ASSERT_EQ(m_data32_out_pegadv, m_data32_in_pegadv);
}


// ----------------------------------------
class Peg2 : public testing::Test {
protected:
    Peg2() {}
    virtual ~Peg2() {}
    virtual void SetUp() {
        m_peg           = new Peg (m_peid, base_pegreg, CORE_G4MH, CORE_VER_11);
    }
    virtual void TearDown() { delete m_peg; }

public:
    Peg          *m_peg;
    MockSideBand sideband;

public:
    uint32_t m_peid = PE0_PEG_UT;
    uint32_t base_pegreg = 0xFFC6C000;
};


/*!
 * @brief Check IsSPCAddr method.
 */

TEST_F(Peg2, Check_IsSPCAddr_1) {
    ASSERT_TRUE( m_peg->IsSPCAddr(Peg::L1RAM_E2X_START));
    ASSERT_TRUE( m_peg->IsSPCAddr(Peg::L1RAM_E2X_END));
    ASSERT_TRUE( m_peg->IsSPCAddr(Peg::L1RAM_E2X_START + 0x4));
}


TEST_F(Peg2, Check_IsSPCAddr_2) {
    ASSERT_FALSE( m_peg->IsSPCAddr(Peg::L1RAM_E2X_START - 0x4));
    ASSERT_FALSE( m_peg->IsSPCAddr(Peg::L1RAM_E2X_END + 0x4));
}


/*!
 * @brief Check IsAddressMatch method.
 * This function is only used to check in area [0xFC000000, 0xFFFFFFFF].
 */

TEST_F(Peg2, Check_IsAddressMatch_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Set BAD = 0xFC00_0000, ADV = 0x0000_0000.
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    ASSERT_TRUE( m_peg->IsAddressMatch(chn_0, 0xFC000000, CORE_G4MH, CORE_VER_11));
    ASSERT_TRUE( m_peg->IsAddressMatch(chn_0, 0xFC000004, CORE_G4MH, CORE_VER_11));
    ASSERT_TRUE( m_peg->IsAddressMatch(chn_0, 0xFC000FFF, CORE_G4MH, CORE_VER_11));
    ASSERT_TRUE( m_peg->IsAddressMatch(chn_0, 0xFC00FFFF, CORE_G4MH, CORE_VER_11));
    ASSERT_TRUE( m_peg->IsAddressMatch(chn_0, 0xFC0FFFFF, CORE_G4MH, CORE_VER_11));
    ASSERT_TRUE( m_peg->IsAddressMatch(chn_0, 0xFCFFFFFF, CORE_G4MH, CORE_VER_11));
    ASSERT_TRUE( m_peg->IsAddressMatch(chn_0, 0xFFFFFFFF, CORE_G4MH, CORE_VER_11));

    // Set ADV = 0x03FFF000,
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);
    ASSERT_TRUE( m_peg->IsAddressMatch(chn_0, 0xFC000000, CORE_G4MH, CORE_VER_11));
    ASSERT_TRUE( m_peg->IsAddressMatch(chn_0, 0xFC000004, CORE_G4MH, CORE_VER_11));
    ASSERT_TRUE( m_peg->IsAddressMatch(chn_0, 0xFC000FFF, CORE_G4MH, CORE_VER_11));
}


TEST_F(Peg2, Check_IsAddressMatch_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Set BAD = 0xFC00_0000.
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    // Set ADV = 0x03FFF000.
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);
    ASSERT_FALSE( m_peg->IsAddressMatch(chn_0, 0xFC00FFFF, CORE_G4MH, CORE_VER_11));
    ASSERT_FALSE( m_peg->IsAddressMatch(chn_0, 0xFC0FFFFF, CORE_G4MH, CORE_VER_11));
    ASSERT_FALSE( m_peg->IsAddressMatch(chn_0, 0xFCFFFFFF, CORE_G4MH, CORE_VER_11));
    ASSERT_FALSE( m_peg->IsAddressMatch(chn_0, 0xFFFFFFFF, CORE_G4MH, CORE_VER_11));
    ASSERT_FALSE( m_peg->IsAddressMatch(chn_0, 0xFFFFFFFF, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsSpidAllowed method.
 */

TEST_F(Peg2, Check_IsSpidAllowed_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable SPID
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetSPID(0);
    ASSERT_FALSE( m_peg->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_11));
    sb_info.SetSPID(31);
    ASSERT_FALSE( m_peg->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_11));
    sb_info.SetSPID(15);
    ASSERT_FALSE( m_peg->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_11));
}


TEST_F(Peg2, Check_IsSpidAllowed_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // Enable SPID
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x01, 0x80, 0x00, 0x80}, SIZE_32BIT);
    sb_info.SetSPID(0);
    ASSERT_TRUE( m_peg->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_11));
    sb_info.SetSPID(31);
    ASSERT_TRUE( m_peg->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_11));
    sb_info.SetSPID(15);
    ASSERT_TRUE( m_peg->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsWriteGloAllowed method.
 */

TEST_F(Peg2, Check_IsWriteGloAllowed_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable WG
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);

    ASSERT_FALSE( m_peg->IsWriteGloAllowed(chn_0, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_FALSE( m_peg->IsWriteGloAllowed(chn_0, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_FALSE( m_peg->IsWriteGloAllowed(chn_0, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_FALSE( m_peg->IsWriteGloAllowed(chn_0, DATA_RMW, CORE_G4MH, CORE_VER_11));
}


TEST_F(Peg2, Check_IsWriteGloAllowed_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Enable WG
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x02, 0x00, 0x00, 0x00}, SIZE_32BIT);

    ASSERT_FALSE( m_peg->IsWriteGloAllowed(chn_0, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_FALSE( m_peg->IsWriteGloAllowed(chn_0, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_FALSE( m_peg->IsWriteGloAllowed(chn_0, DATA_RMW, CORE_G4MH, CORE_VER_11));
}


TEST_F(Peg2, Check_IsWriteGloAllowed_3) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // Enable WG
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x02, 0x00, 0x00, 0x00}, SIZE_32BIT);

    ASSERT_TRUE( m_peg->IsWriteGloAllowed(chn_0, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsReadGloAllowed method.
 */

TEST_F(Peg2, Check_IsReadGloAllowed_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable RG
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);

    ASSERT_FALSE( m_peg->IsReadGloAllowed(chn_0, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_FALSE( m_peg->IsReadGloAllowed(chn_0, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_FALSE( m_peg->IsReadGloAllowed(chn_0, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_FALSE( m_peg->IsReadGloAllowed(chn_0, DATA_RMW, CORE_G4MH, CORE_VER_11));
}


TEST_F(Peg2, Check_IsReadGloAllowed_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Enable RG
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x01, 0x00, 0x00, 0x00}, SIZE_32BIT);

    ASSERT_FALSE( m_peg->IsReadGloAllowed(chn_0, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_FALSE( m_peg->IsReadGloAllowed(chn_0, DATA_RMW, CORE_G4MH, CORE_VER_11));
}


TEST_F(Peg2, Check_IsReadGloAllowed_3) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // Enable RG
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x01, 0x00, 0x00, 0x00}, SIZE_32BIT);

    ASSERT_TRUE( m_peg->IsReadGloAllowed(chn_0, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_TRUE( m_peg->IsReadGloAllowed(chn_0, INST_READ, CORE_G4MH, CORE_VER_11));

}


/*!
 * @brief Check IsUMDisabled method.
 */

TEST_F(Peg2, Check_IsUMDisabled_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // Enable UME
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x10, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetUM(0);
    ASSERT_FALSE( m_peg->IsUMDisabled(chn_0, sb_info, CORE_G4MH, CORE_VER_11));
    sb_info.SetUM(1);
    ASSERT_FALSE( m_peg->IsUMDisabled(chn_0, sb_info, CORE_G4MH, CORE_VER_11));
}


TEST_F(Peg2, Check_IsUMDisabled_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable UME
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetUM(0);
    ASSERT_FALSE( m_peg->IsUMDisabled(chn_0, sb_info, CORE_G4MH, CORE_VER_11));
}


TEST_F(Peg2, Check_IsUMDisabled_3) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable UME
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetUM(1);
    ASSERT_TRUE( m_peg->IsUMDisabled(chn_0, sb_info, CORE_G4MH, CORE_VER_11));

}


/*!
 * @brief Check IsDbgAllowed method.
 */

TEST_F(Peg2, Check_IsDbgAllowed_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable DB permission
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetDM(0);
    ASSERT_FALSE( m_peg->IsDbgAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_11));
    sb_info.SetDM(1);
    ASSERT_FALSE( m_peg->IsDbgAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsDbgAllowed method.
 */

TEST_F(Peg2, Check_IsDbgAllowed_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // Enable DB permission
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x40, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetDM(0);
    ASSERT_FALSE( m_peg->IsDbgAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsDbgAllowed method.
 */

TEST_F(Peg2, Check_IsDbgAllowed_3) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x40, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetDM(1);
    ASSERT_TRUE( m_peg->IsDbgAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_11));
}


// ----------------------------------------
// Peg3 is used to checked IsMemAccPermit in case PE of memory access is PE contains PEG.

class Peg3 : public testing::Test {
protected:
    Peg3() {}
    virtual ~Peg3() {}
    virtual void SetUp() {
        m_peg           = new Peg (m_peid, base_pegreg, CORE_G4MH, CORE_VER_11);
    }
    virtual void TearDown() { delete m_peg; }

public:
    Peg          *m_peg;
    MockSideBand sideband;

public:
    uint32_t m_peid = PE0_PEG_UT;
    uint32_t base_pegreg = 0xFFC6C000;
};


/*!
 * @brief Check IsMemAccPermit method.
 * case 1: Guard Setting is not enable.
 * Return SYSERR_NONE
 */

TEST_F(Peg3, Check_IsMemAccPermit_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(false);
    // Don't need to enable channel 0 because the function returns in previous condition.
//    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_0, true);
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 2: In case read type, RMW instruction (include NOT1/CLR1/SET1/CAXI) is ignored
 * Return SYSERR_NONE
 */

TEST_F(Peg3, Check_IsMemAccPermit_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    // Don't need to enable channel 0 because the function returns in previous condition.
//    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_0, true);
    sb_info.SetBitOp(SideBand::BITOP_SET);
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    sb_info.SetBitOp(SideBand::BITOP_CLR);
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    sb_info.SetBitOp(SideBand::BITOP_NOT);
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    sb_info.SetBitOp(SideBand::BITOP_CAXI);
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 3: In case the access is self PE, only INST_READ is checked.
 * Return SYSERR_NONE
 */

TEST_F(Peg3, Check_IsMemAccPermit_3) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    // Don't need to enable channel 0 because the function returns in previous condition.
//    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_0, true);
    sb_info.SetBMID(0x0);
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


// ----------------------------------------
class Peg4 : public testing::Test {
protected:
    Peg4() {}
    virtual ~Peg4() {}
    virtual void SetUp() {
        m_peg           = new Peg (m_peid, base_pegreg, CORE_G4MH, CORE_VER_11);
    }
    virtual void TearDown() { delete m_peg; }

public:
    Peg          *m_peg;
    MockSideBand sideband;

public:
    uint32_t m_peid = PE1_PEG_UT;
    uint32_t base_pegreg = 0xFFC6C000;
};


/*!
 * @brief Check IsMemAccPermit method.
 * case 4.1: Address at channel 0 is not match.
 * Return SYSERR_SEG
 */

TEST_F(Peg4, Check_IsMemAccPermit_AddrMatch_ch0) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_0, true);
    sb_info.SetBitOp(SideBand::BITOP_NONE);
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 4.2: Address at channel 1 is not match.
 * Return SYSERR_SEG
 */

TEST_F(Peg4, Check_IsMemAccPermit_AddrMatch_ch1) {
    SideBand sb_info;
    uint32_t chn_1 = 0x1;
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_1, true);
    sb_info.SetBitOp(SideBand::BITOP_NONE);
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 4.3: Address at channel 2 is not match.
 * Return SYSERR_SEG
 */

TEST_F(Peg4, Check_IsMemAccPermit_AddrMatch_ch2) {
    SideBand sb_info;
    uint32_t chn_2 = 0x2;
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_2, true);
    sb_info.SetBitOp(SideBand::BITOP_NONE);
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 4.4: Address at channel 3 is not match.
 * Return SYSERR_SEG
 */

TEST_F(Peg4, Check_IsMemAccPermit_AddrMatch_ch3) {
    SideBand sb_info;
    uint32_t chn_3 = 0x3;
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_3, true);
    sb_info.SetBitOp(SideBand::BITOP_NONE);
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 5.1: DB permission at channel 0 is match.
 * Return SYSERR_NONE
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsDbgAllowed_ch0) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);

    // Enable Debug permision
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x40, 0x00, 0x00, 0x00}, SIZE_32BIT);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_0, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(1);

    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 5.2: DB permission at channel 1 is match.
 * Return SYSERR_NONE
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsDbgAllowed_ch1) {
    SideBand sb_info;
    uint32_t chn_1 = 0x1;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);

    // Enable Debug permision
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x40, 0x00, 0x00, 0x00}, SIZE_32BIT);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_1, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(1);

    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 5.3: DB permission at channel 2 is match.
 * Return SYSERR_NONE
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsDbgAllowed_ch2) {
    SideBand sb_info;
    uint32_t chn_2 = 0x2;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);

    // Enable Debug permision
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x40, 0x00, 0x00, 0x00}, SIZE_32BIT);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_2, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(1);

    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 5.4: DB permission at channel 3 is match.
 * Return SYSERR_NONE
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsDbgAllowed_ch3) {
    SideBand sb_info;
    uint32_t chn_3 = 0x3;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);

    // Enable Debug permision
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x40, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_3, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(1);

    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 6.1: UM permission at channel 0 is not match.
 * Return SYSERR_SEG
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsUMAllowed_ch0) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_0, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // UM is disable.
    sb_info.SetUM(1);
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 6.2: UM permission at channel 1 is not match.
 * Return SYSERR_SEG
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsUMAllowed_ch1) {
    SideBand sb_info;
    uint32_t chn_1 = 0x1;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x00, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_1, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // UM is disable.
    sb_info.SetUM(1);
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 6.3: UM permission at channel 2 is not match.
 * Return SYSERR_SEG
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsUMAllowed_ch2) {
    SideBand sb_info;
    uint32_t chn_2 = 0x2;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x00, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_2, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // UM is disable.
    sb_info.SetUM(1);
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 6.4: UM permission at channel 3 is not match.
 * Return SYSERR_SEG
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsUMAllowed_ch3) {
    SideBand sb_info;
    uint32_t chn_3 = 0x3;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x00, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_3, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // UM is disable.
    sb_info.SetUM(1);
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 7.1: SPID at channel 0 is match.
 * Return SYSERR_NONE
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsSPIDllowed_ch0) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Enable SPID
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x02, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_0, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 8.1: WG at channel 0 is match.
 * Return SYSERR_NONE
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsWriteGloAllowed_ch0) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable SPID
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x02, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_0, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));

    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));

    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 9.1: RG at channel 0 is match.
 * Return SYSERR_NONE
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsReadGloAllowed_ch0) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable SPID
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x01, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_0, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));

    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));

    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 7.2: SPID at channel 1 is match.
 * Return SYSERR_NONE
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsSPIDllowed_ch1) {
    SideBand sb_info;
    uint32_t chn_1 = 0x1;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Enable SPID
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x02, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x00, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_1, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 8.2: WG at channel 1 is match.
 * Return SYSERR_NONE
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsWriteGloAllowed_ch1) {
    SideBand sb_info;
    uint32_t chn_1 = 0x1;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable SPID
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x02, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_1, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));

    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));

    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 9.2: RG at channel 1 is match.
 * Return SYSERR_NONE
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsReadGloAllowed_ch1) {
    SideBand sb_info;
    uint32_t chn_1 = 0x1;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable SPID
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_1,
                                    new uint8_t[4] {0x01, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_1, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));

    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));

    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 7.3: SPID at channel 2 is match.
 * Return SYSERR_NONE
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsSPIDllowed_ch2) {
    SideBand sb_info;
    uint32_t chn_2 = 0x2;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Enable SPID
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x02, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x00, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_2, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 8.3: WG at channel 2 is match.
 * Return SYSERR_NONE
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsWriteGloAllowed_ch2) {
    SideBand sb_info;
    uint32_t chn_2 = 0x2;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable SPID
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x02, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_2, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));

    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));

    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 9.3: RG at channel 2 is match.
 * Return SYSERR_NONE
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsReadGloAllowed_ch2) {
    SideBand sb_info;
    uint32_t chn_2 = 0x2;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable SPID
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_2,
                                    new uint8_t[4] {0x01, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_2, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));

    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));

    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 7.4: SPID at channel 3 is match.
 * Return SYSERR_NONE
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsSPIDllowed_ch3) {
    SideBand sb_info;
    uint32_t chn_3 = 0x3;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Enable SPID
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x02, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x00, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_3, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 8.4: WG at channel 3 is match.
 * Return SYSERR_NONE
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsWriteGloAllowed_ch3) {
    SideBand sb_info;
    uint32_t chn_3 = 0x3;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable SPID
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x02, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_3, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));

    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));

    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 9.4: RG at channel 3 is match.
 * Return SYSERR_NONE
 */

TEST_F(Peg4, Check_IsMemAccPermit_IsReadGloAllowed_ch3) {
    SideBand sb_info;
    uint32_t chn_3 = 0x3;

    // BAD, ADV is satisfied
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable SPID
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_peg->TargetWriteDebug(base_pegreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_3,
                                    new uint8_t[4] {0x01, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->Reset();
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetGuardEn(true);
    m_peg->GetPegCtrl()->GetPegCtrlReg()->GetPegInternal()->SetChannelEn(chn_3, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_PEG, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_11));

    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_11));

    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
    ASSERT_EQ(SYSERR_NONE, m_peg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_11));
}


// ----------------------------------------
class Peg5 : public testing::Test {
protected:
    Peg5() {}
    virtual ~Peg5() {}
    virtual void SetUp() {
        m_peg           = new Peg (m_peid, base_pegreg, CORE_G4MH, CORE_VER_11);
    }
    virtual void TearDown() { delete m_peg; }

public:
    Peg          *m_peg;
    MockSideBand sideband;

public:
    uint32_t m_peid = PE0_PEG_UT;
    uint32_t base_pegreg = 0xFFC6C000;
};


/*!
 * @brief Check CauseError method.
 * case 1:
 * Return SYSERR_PEG_S
 */

TEST_F(Peg5, Check_CauseError_SamePE_InstRead_S_Input_SYSERR_PEG_M) {
    uint32_t err_dm = 0x1, err_um = 0x1, err_spid = 0xa;
    uint32_t err_addr = 0xFFFFFFFF, err_addr2 = 0xcafecafe;
    SideBand sb_info;
    sb_info.SetBMID(0x0);
    sb_info.SetSPID(err_spid);
    sb_info.SetDM(err_dm);
    sb_info.SetUM(err_um);

    ASSERT_EQ(SYSERR_PEG_S, m_peg->CauseError(sb_info, err_addr,
                           INST_READ, CORE_G4MH, CORE_VER_11, SYSERR_PEG_M));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());

    // First error
    ASSERT_EQ(0xFFC6C830, m_peg->GetPegErrCap()->m_reg_addr_base);

    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetSEC());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetERR());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_peg->GetPegErrCap()->GetPegErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_peg->GetPegErrCap()->GetPegErrType()->GetDBG());
    ASSERT_EQ(err_um, m_peg->GetPegErrCap()->GetPegErrType()->GetUM());
    ASSERT_EQ(err_addr, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());

    ASSERT_EQ(SYSERR_PEG_S, m_peg->CauseError(sb_info, err_addr2,
                           INST_READ, CORE_G4MH, CORE_VER_11, SYSERR_PEG_M));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());
    ASSERT_EQ(0xFFC6C830, m_peg->GetPegErrCap()->m_reg_addr_base);

    // Second error: Update OVF
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetSEC());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetERR());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_peg->GetPegErrCap()->GetPegErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_peg->GetPegErrCap()->GetPegErrType()->GetDBG());
    ASSERT_EQ(err_um, m_peg->GetPegErrCap()->GetPegErrType()->GetUM());
    ASSERT_NE(err_addr2, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());
    ASSERT_EQ(err_addr, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());
}


/*!
 * @brief Check CauseError method.
 * case 2:
 * Return SYSERR_PEG_S
 */

TEST_F(Peg5, Check_CauseError_SamePE_InstRead_S_Input_SYSERR_PEG_S) {
    uint32_t err_dm = 0x1, err_um = 0x1, err_spid = 0xa;
    uint32_t err_addr = 0xFFFFFFFF, err_addr2 = 0xcafecafe;
    SideBand sb_info;
    sb_info.SetBMID(0x0);
    sb_info.SetSPID(err_spid);
    sb_info.SetDM(err_dm);
    sb_info.SetUM(err_um);

    ASSERT_EQ(SYSERR_PEG_S, m_peg->CauseError(sb_info, err_addr,
                           INST_READ, CORE_G4MH, CORE_VER_11, SYSERR_PEG_S));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());

    // First error
    ASSERT_EQ(0xFFC6C830, m_peg->GetPegErrCap()->m_reg_addr_base);

    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetSEC());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetERR());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_peg->GetPegErrCap()->GetPegErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_peg->GetPegErrCap()->GetPegErrType()->GetDBG());
    ASSERT_EQ(err_um, m_peg->GetPegErrCap()->GetPegErrType()->GetUM());
    ASSERT_EQ(err_addr, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());

    ASSERT_EQ(SYSERR_PEG_S, m_peg->CauseError(sb_info, err_addr2,
                           INST_READ, CORE_G4MH, CORE_VER_11, SYSERR_PEG_S));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());
    ASSERT_EQ(0xFFC6C830, m_peg->GetPegErrCap()->m_reg_addr_base);

    // Second error: Update OVF
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetSEC());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetERR());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_peg->GetPegErrCap()->GetPegErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_peg->GetPegErrCap()->GetPegErrType()->GetDBG());
    ASSERT_EQ(err_um, m_peg->GetPegErrCap()->GetPegErrType()->GetUM());
    ASSERT_NE(err_addr2, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());
    ASSERT_EQ(err_addr, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());
}


/*!
 * @brief Check CauseError method.
 * case 2:
 * Return SYSERR_NONE
 */

TEST_F(Peg5, Check_CauseError_SamePE_DataReadWrite) {
    SideBand sb_info;
    sb_info.SetBMID(0x0);
    // In case input is SYSERR_PEG_M
    ASSERT_EQ(SYSERR_NONE, m_peg->CauseError(sb_info, 0xFFFFFFFF,
                           DATA_WRITE, CORE_G4MH, CORE_VER_11, SYSERR_PEG_M));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());
    ASSERT_EQ(0xFFFFFFFF, m_peg->GetPegErrCap()->m_reg_addr_base);

    ASSERT_EQ(SYSERR_NONE, m_peg->CauseError(sb_info, 0xFFFFFFFF,
                           DATA_READ, CORE_G4MH, CORE_VER_11, SYSERR_PEG_M));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());
    ASSERT_EQ(0xFFFFFFFF, m_peg->GetPegErrCap()->m_reg_addr_base);

    // In case input is SYSERR_PEG_S
    ASSERT_EQ(SYSERR_NONE, m_peg->CauseError(sb_info, 0xFFFFFFFF,
                           DATA_WRITE, CORE_G4MH, CORE_VER_11, SYSERR_PEG_S));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());
    ASSERT_EQ(0xFFFFFFFF, m_peg->GetPegErrCap()->m_reg_addr_base);

    ASSERT_EQ(SYSERR_NONE, m_peg->CauseError(sb_info, 0xFFFFFFFF,
                           DATA_READ, CORE_G4MH, CORE_VER_11, SYSERR_PEG_S));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());
    ASSERT_EQ(0xFFFFFFFF, m_peg->GetPegErrCap()->m_reg_addr_base);
}


// ----------------------------------------
class Peg6 : public testing::Test {
protected:
    Peg6() {}
    virtual ~Peg6() {}
    virtual void SetUp() {
        m_peg           = new Peg (m_peid, base_pegreg, CORE_G4MH, CORE_VER_11);
    }
    virtual void TearDown() { delete m_peg; }

public:
    Peg          *m_peg;
    MockSideBand sideband;

public:
    uint32_t m_peid = PE1_PEG_UT;
    uint32_t base_pegreg = 0xFFC6C000;
};


/*!
 * @brief Check CauseError method.
 * case 3:
 * Return SYSERR_PEG_M
 */


TEST_F(Peg6, Check_CauseError_DiffPE_InstRead_M_Input_SYSERR_PEG_M) {
    uint32_t err_dm = 0x1, err_um = 0x1, err_spid = 0xa;
    uint32_t err_addr = 0xFFFFFFFF, err_addr2 = 0xcafecafe;
    SideBand sb_info;
    sb_info.SetBMID(0x0);
    sb_info.SetSPID(err_spid);
    sb_info.SetDM(err_dm);
    sb_info.SetUM(err_um);

    ASSERT_EQ(SYSERR_PEG_M, m_peg->CauseError(sb_info, err_addr,
                           INST_READ, CORE_G4MH, CORE_VER_11, SYSERR_PEG_M));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());

    // First error
    ASSERT_EQ(0xFFC6C820, m_peg->GetPegErrCap()->m_reg_addr_base);

    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetSEC());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetERR());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_peg->GetPegErrCap()->GetPegErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_peg->GetPegErrCap()->GetPegErrType()->GetDBG());
    ASSERT_EQ(err_um, m_peg->GetPegErrCap()->GetPegErrType()->GetUM());
    ASSERT_EQ(err_addr, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());

    ASSERT_EQ(SYSERR_PEG_M, m_peg->CauseError(sb_info, err_addr2,
                           INST_READ, CORE_G4MH, CORE_VER_11, SYSERR_PEG_M));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());
    ASSERT_EQ(0xFFC6C820, m_peg->GetPegErrCap()->m_reg_addr_base);

    // Second error: Update OVF
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetSEC());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetERR());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_peg->GetPegErrCap()->GetPegErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_peg->GetPegErrCap()->GetPegErrType()->GetDBG());
    ASSERT_EQ(err_um, m_peg->GetPegErrCap()->GetPegErrType()->GetUM());
    ASSERT_NE(err_addr2, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());
    ASSERT_EQ(err_addr, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());
}


/*!
 * @brief Check CauseError method.
 * case 4:
 * Return SYSERR_PEG_M
 */

TEST_F(Peg6, Check_CauseError_DiffPE_DataRead_M_Input_SYSERR_PEG_M) {
    uint32_t err_dm = 0x1, err_um = 0x1, err_spid = 0xa;
    uint32_t err_addr = 0xFFFFFFFF, err_addr2 = 0xcafecafe;
    SideBand sb_info;
    sb_info.SetBMID(0x0);
    sb_info.SetSPID(err_spid);
    sb_info.SetDM(err_dm);
    sb_info.SetUM(err_um);

    ASSERT_EQ(SYSERR_PEG_M, m_peg->CauseError(sb_info, err_addr,
                           DATA_READ, CORE_G4MH, CORE_VER_11, SYSERR_PEG_M));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());

    // First error
    ASSERT_EQ(0xFFC6C820, m_peg->GetPegErrCap()->m_reg_addr_base);

    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetSEC());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetERR());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_peg->GetPegErrCap()->GetPegErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_peg->GetPegErrCap()->GetPegErrType()->GetDBG());
    ASSERT_EQ(err_um, m_peg->GetPegErrCap()->GetPegErrType()->GetUM());
    ASSERT_EQ(err_addr, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());

    ASSERT_EQ(SYSERR_PEG_M, m_peg->CauseError(sb_info, err_addr2,
                           DATA_READ, CORE_G4MH, CORE_VER_11, SYSERR_PEG_M));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());
    ASSERT_EQ(0xFFC6C820, m_peg->GetPegErrCap()->m_reg_addr_base);

    // Second error: Update OVF
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetSEC());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetERR());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_peg->GetPegErrCap()->GetPegErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_peg->GetPegErrCap()->GetPegErrType()->GetDBG());
    ASSERT_EQ(err_um, m_peg->GetPegErrCap()->GetPegErrType()->GetUM());
    ASSERT_NE(err_addr2, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());
    ASSERT_EQ(err_addr, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());
}


/*!
 * @brief Check CauseError method.
 * case 5:
 * Return SYSERR_PEG_M
 */

TEST_F(Peg6, Check_CauseError_DiffPE_DataWrite_M_Input_SYSERR_PEG_M) {
    uint32_t err_dm = 0x1, err_um = 0x1, err_spid = 0xa;
    uint32_t err_addr = 0xFFFFFFFF, err_addr2 = 0xcafecafe;
    SideBand sb_info;
    sb_info.SetBMID(0x0);
    sb_info.SetSPID(err_spid);
    sb_info.SetDM(err_dm);
    sb_info.SetUM(err_um);

    ASSERT_EQ(SYSERR_PEG_M, m_peg->CauseError(sb_info, err_addr,
                           DATA_WRITE, CORE_G4MH, CORE_VER_11, SYSERR_PEG_M));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());

    // First error
    ASSERT_EQ(0xFFC6C820, m_peg->GetPegErrCap()->m_reg_addr_base);

    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetSEC());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetERR());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrStat()->GetOVF());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_peg->GetPegErrCap()->GetPegErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_peg->GetPegErrCap()->GetPegErrType()->GetDBG());
    ASSERT_EQ(err_um, m_peg->GetPegErrCap()->GetPegErrType()->GetUM());
    ASSERT_EQ(err_addr, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());

    ASSERT_EQ(SYSERR_PEG_M, m_peg->CauseError(sb_info, err_addr2,
                           DATA_WRITE, CORE_G4MH, CORE_VER_11, SYSERR_PEG_M));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());
    ASSERT_EQ(0xFFC6C820, m_peg->GetPegErrCap()->m_reg_addr_base);

    // Second error: Update OVF
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetSEC());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetERR());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetOVF());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_peg->GetPegErrCap()->GetPegErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_peg->GetPegErrCap()->GetPegErrType()->GetDBG());
    ASSERT_EQ(err_um, m_peg->GetPegErrCap()->GetPegErrType()->GetUM());
    ASSERT_NE(err_addr2, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());
    ASSERT_EQ(err_addr, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());
}





/*!
 * @brief Check CauseError method.
 * case 6:
 * Return SYSERR_PEG_S
 */

TEST_F(Peg6, Check_CauseError_DiffPE_InstRead_S_Input_SYSERR_PEG_S) {
    uint32_t err_dm = 0x1, err_um = 0x1, err_spid = 0xa;
    uint32_t err_addr = 0xFFFFFFFF, err_addr2 = 0xcafecafe;
    SideBand sb_info;
    sb_info.SetBMID(0x0);
    sb_info.SetSPID(err_spid);
    sb_info.SetDM(err_dm);
    sb_info.SetUM(err_um);

    ASSERT_EQ(SYSERR_PEG_S, m_peg->CauseError(sb_info, err_addr,
                           INST_READ, CORE_G4MH, CORE_VER_11, SYSERR_PEG_S));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());

    // First error
    ASSERT_EQ(0xFFC6C830, m_peg->GetPegErrCap()->m_reg_addr_base);

    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetSEC());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetERR());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_peg->GetPegErrCap()->GetPegErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_peg->GetPegErrCap()->GetPegErrType()->GetDBG());
    ASSERT_EQ(err_um, m_peg->GetPegErrCap()->GetPegErrType()->GetUM());
    ASSERT_EQ(err_addr, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());

    ASSERT_EQ(SYSERR_PEG_S, m_peg->CauseError(sb_info, err_addr2,
                           INST_READ, CORE_G4MH, CORE_VER_11, SYSERR_PEG_S));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());
    ASSERT_EQ(0xFFC6C830, m_peg->GetPegErrCap()->m_reg_addr_base);

    // Second error: Update OVF
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetSEC());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetERR());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_peg->GetPegErrCap()->GetPegErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_peg->GetPegErrCap()->GetPegErrType()->GetDBG());
    ASSERT_EQ(err_um, m_peg->GetPegErrCap()->GetPegErrType()->GetUM());
    ASSERT_NE(err_addr2, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());
    ASSERT_EQ(err_addr, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());
}


/*!
 * @brief Check CauseError method.
 * case 7:
 * Return SYSERR_PEG_S
 */

TEST_F(Peg6, Check_CauseError_DiffPE_DataRead_S_Input_SYSERR_PEG_S) {
    uint32_t err_dm = 0x1, err_um = 0x1, err_spid = 0xa;
    uint32_t err_addr = 0xFFFFFFFF, err_addr2 = 0xcafecafe;
    SideBand sb_info;
    sb_info.SetBMID(0x0);
    sb_info.SetSPID(err_spid);
    sb_info.SetDM(err_dm);
    sb_info.SetUM(err_um);

    ASSERT_EQ(SYSERR_PEG_S, m_peg->CauseError(sb_info, err_addr,
                           DATA_READ, CORE_G4MH, CORE_VER_11, SYSERR_PEG_S));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());

    // First error
    ASSERT_EQ(0xFFC6C830, m_peg->GetPegErrCap()->m_reg_addr_base);

    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetSEC());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetERR());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_peg->GetPegErrCap()->GetPegErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_peg->GetPegErrCap()->GetPegErrType()->GetDBG());
    ASSERT_EQ(err_um, m_peg->GetPegErrCap()->GetPegErrType()->GetUM());
    ASSERT_EQ(err_addr, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());

    ASSERT_EQ(SYSERR_PEG_S, m_peg->CauseError(sb_info, err_addr2,
                           DATA_READ, CORE_G4MH, CORE_VER_11, SYSERR_PEG_S));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());
    ASSERT_EQ(0xFFC6C830, m_peg->GetPegErrCap()->m_reg_addr_base);

    // Second error: Update OVF
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetSEC());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetERR());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_peg->GetPegErrCap()->GetPegErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_peg->GetPegErrCap()->GetPegErrType()->GetDBG());
    ASSERT_EQ(err_um, m_peg->GetPegErrCap()->GetPegErrType()->GetUM());
    ASSERT_NE(err_addr2, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());
    ASSERT_EQ(err_addr, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());
}


/*!
 * @brief Check CauseError method.
 * case 8:
 * Return SYSERR_PEG_S
 */

TEST_F(Peg6, Check_CauseError_DiffPE_DataWrite_S_Input_SYSERR_PEG_S) {
    uint32_t err_dm = 0x1, err_um = 0x1, err_spid = 0xa;
    uint32_t err_addr = 0xFFFFFFFF, err_addr2 = 0xcafecafe;
    SideBand sb_info;
    sb_info.SetBMID(0x0);
    sb_info.SetSPID(err_spid);
    sb_info.SetDM(err_dm);
    sb_info.SetUM(err_um);

    ASSERT_EQ(SYSERR_PEG_S, m_peg->CauseError(sb_info, err_addr,
                           DATA_WRITE, CORE_G4MH, CORE_VER_11, SYSERR_PEG_S));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());

    // First error
    ASSERT_EQ(0xFFC6C830, m_peg->GetPegErrCap()->m_reg_addr_base);

    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetSEC());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetERR());
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrStat()->GetOVF());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_peg->GetPegErrCap()->GetPegErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_peg->GetPegErrCap()->GetPegErrType()->GetDBG());
    ASSERT_EQ(err_um, m_peg->GetPegErrCap()->GetPegErrType()->GetUM());
    ASSERT_EQ(err_addr, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());

    ASSERT_EQ(SYSERR_PEG_S, m_peg->CauseError(sb_info, err_addr2,
                           DATA_WRITE, CORE_G4MH, CORE_VER_11, SYSERR_PEG_S));
    ASSERT_NE(nullptr, m_peg->GetPegErrCap());
    ASSERT_EQ(0xFFC6C830, m_peg->GetPegErrCap()->m_reg_addr_base);

    // Second error: Update OVF
    ASSERT_EQ(0x0U, m_peg->GetPegErrCap()->GetPegErrType()->GetSEC());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetERR());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrStat()->GetOVF());
    ASSERT_EQ(0x1U, m_peg->GetPegErrCap()->GetPegErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_peg->GetPegErrCap()->GetPegErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_peg->GetPegErrCap()->GetPegErrType()->GetDBG());
    ASSERT_EQ(err_um, m_peg->GetPegErrCap()->GetPegErrType()->GetUM());
    ASSERT_NE(err_addr2, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());
    ASSERT_EQ(err_addr, m_peg->GetPegErrCap()->GetPegErrAddr()->GetAddr());
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
