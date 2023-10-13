/**
 * @file test_pegctrl.cpp
 *
 * @date Created on: 2016
 * @author Author: Quang Lam <quang.lam.px@rvc.renesas.com>
 * @brief UNIT TEST for behavior of PEG control. (SAMPLE)
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

class PegCtrl;

// ----------------------------------------
class PegCtrl1 : public testing::Test {
protected:
    PegCtrl1() {}
    virtual ~PegCtrl1() {}
    virtual void SetUp() {
        m_pegctrl           = new PegCtrl (m_peid, base_pegctrlreg, CORE_G4MH, CORE_VER_20);
    }
    virtual void TearDown() { delete m_pegctrl; }

public:
    PegCtrl          *m_pegctrl;
    MockSideBand sideband;

public:
    uint32_t m_peid = 0x0;
    uint32_t base_pegctrlreg = 0xFFC6C000;
    union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };
};


/*!
 * @brief Check Initial method.
 */

TEST_F(PegCtrl1, Check_Initial_Setup) {
    ASSERT_NE(nullptr, m_pegctrl->GetPegCtrlReg());
    ASSERT_FALSE(m_pegctrl->GetPegCtrlReg()->GetPegInternal()->IsGuardEn());
}


/*!
 * @brief Check Reset method.
 */
TEST_F(PegCtrl1, Check_Reset) {
    SideBand sb_info;
    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT);
    ASSERT_EQ(0x1U, m_pegctrl->GetPegCtrlReg()->GetPegKceProt()->GetKCE());
    m_pegctrl->HardReset();
    ASSERT_EQ(0x0U, m_pegctrl->GetPegCtrlReg()->GetPegKceProt()->GetKCE());
}


/*!
 * @brief Check TargetWrite method.
 */
TEST_F(PegCtrl1, Check_TargetWrite) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT);

    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x53, 0x01, 0x00, 0x00}, SIZE_32BIT);
    // at least, 1 PEG channel setting is enable.

    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);

    ASSERT_EQ(0x1U, m_pegctrl->GetPegCtrlReg()->GetPegKceProt()->GetKCE());
    ASSERT_EQ(0x1U, m_pegctrl->GetPegCtrlReg()->GetPegProt(chn_0)->GetGEN());
    ASSERT_EQ(0xFFFFFFFFU, m_pegctrl->GetPegCtrlReg()->GetPegSpid(chn_0)->GetSPID());
    ASSERT_EQ(0xFFFFF000U, m_pegctrl->GetPegCtrlReg()->GetPegBad(chn_0)->GetBAD());
    ASSERT_EQ(0x03FFF000U, m_pegctrl->GetPegCtrlReg()->GetPegAdv(chn_0)->GetADV());

    // Check guard internal value.
    m_pegctrl->GetPegCtrlReg()->GetPegProt(0)->SetGEN(1);
    m_pegctrl->GetPegCtrlReg()->GetPegProt(1)->SetGEN(1);
    m_pegctrl->GetPegCtrlReg()->GetPegProt(2)->SetGEN(1);
    m_pegctrl->GetPegCtrlReg()->GetPegProt(3)->SetGEN(1);
    ASSERT_NO_THROW(m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT));
    ASSERT_TRUE(m_pegctrl->GetPegCtrlReg()->GetPegInternal()->IsChannelEn(0));
    ASSERT_TRUE(m_pegctrl->GetPegCtrlReg()->GetPegInternal()->IsChannelEn(1));
    ASSERT_TRUE(m_pegctrl->GetPegCtrlReg()->GetPegInternal()->IsChannelEn(2));
    ASSERT_TRUE(m_pegctrl->GetPegCtrlReg()->GetPegInternal()->IsChannelEn(3));
    ASSERT_TRUE(m_pegctrl->GetPegCtrlReg()->GetPegInternal()->IsGuardEn());
}


/*!
 * @brief Check TargetRead method.
 */
TEST_F(PegCtrl1, Check_TargetRead) {
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

    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, data_8_input_pegkcprot, SIZE_32BIT);
    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegprot, SIZE_32BIT);
    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegspid, SIZE_32BIT);
    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegbad, SIZE_32BIT);
    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegadv, SIZE_32BIT);

    m_pegctrl->TargetRead(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, data_8_output_pegkcprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegkcprot, sizeof(m_data32));
    uint32_t m_data32_out_pegkcprot = m_data32;
    // Get KCE key in PEGKCPROT.
    memcpy (&m_data32, data_8_input_pegkcprot, sizeof(m_data32));
    uint32_t m_data32_in_pegkcprot = m_data32 & 0x01;;
    ASSERT_EQ(m_data32_out_pegkcprot, m_data32_in_pegkcprot);

    m_pegctrl->TargetRead(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegprot, sizeof(m_data32));
    uint32_t m_data32_out_pegprot = m_data32;
    memcpy (&m_data32, data_8_input_pegprot, sizeof(m_data32));
    uint32_t m_data32_in_pegprot = m_data32;
    ASSERT_EQ(m_data32_out_pegprot, m_data32_in_pegprot);

    m_pegctrl->TargetRead(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegspid, sizeof(m_data32));
    uint32_t m_data32_out_pegspid = m_data32;
    memcpy (&m_data32, data_8_input_pegspid, sizeof(m_data32));
    uint32_t m_data32_in_pegspid = m_data32;
    ASSERT_EQ(m_data32_out_pegspid, m_data32_in_pegspid);

    m_pegctrl->TargetRead(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegbad, sizeof(m_data32));
    uint32_t m_data32_out_pegbad = m_data32;
    memcpy (&m_data32, data_8_input_pegbad, sizeof(m_data32));
    uint32_t m_data32_in_pegbad = m_data32;
    ASSERT_EQ(m_data32_out_pegbad, m_data32_in_pegbad);

    m_pegctrl->TargetRead(sb_info,  base_pegctrlreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegadv, sizeof(m_data32));
    uint32_t m_data32_out_pegadv = m_data32;
    memcpy (&m_data32, data_8_input_pegadv, sizeof(m_data32));
    uint32_t m_data32_in_pegadv = m_data32;
    ASSERT_EQ(m_data32_out_pegadv, m_data32_in_pegadv);
}


/*!
 * @brief Check TargetFetch method.
 */
TEST_F(PegCtrl1, Check_TargetFetch) {
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

    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, data_8_input_pegkcprot, SIZE_32BIT);
    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegprot, SIZE_32BIT);
    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegspid, SIZE_32BIT);
    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegbad, SIZE_32BIT);
    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegadv, SIZE_32BIT);

    m_pegctrl->TargetFetch(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, data_8_output_pegkcprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegkcprot, sizeof(m_data32));
    uint32_t m_data32_out_pegkcprot = m_data32;
    // Get KCE key in PEGKCPROT.
    memcpy (&m_data32, data_8_input_pegkcprot, sizeof(m_data32));
    uint32_t m_data32_in_pegkcprot = m_data32 & 0x01;;
    ASSERT_EQ(m_data32_out_pegkcprot, m_data32_in_pegkcprot);

    m_pegctrl->TargetFetch(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegprot, sizeof(m_data32));
    uint32_t m_data32_out_pegprot = m_data32;
    memcpy (&m_data32, data_8_input_pegprot, sizeof(m_data32));
    uint32_t m_data32_in_pegprot = m_data32;
    ASSERT_EQ(m_data32_out_pegprot, m_data32_in_pegprot);

    m_pegctrl->TargetFetch(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegspid, sizeof(m_data32));
    uint32_t m_data32_out_pegspid = m_data32;
    memcpy (&m_data32, data_8_input_pegspid, sizeof(m_data32));
    uint32_t m_data32_in_pegspid = m_data32;
    ASSERT_EQ(m_data32_out_pegspid, m_data32_in_pegspid);

    m_pegctrl->TargetFetch(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegbad, sizeof(m_data32));
    uint32_t m_data32_out_pegbad = m_data32;
    memcpy (&m_data32, data_8_input_pegbad, sizeof(m_data32));
    uint32_t m_data32_in_pegbad = m_data32;
    ASSERT_EQ(m_data32_out_pegbad, m_data32_in_pegbad);

    m_pegctrl->TargetFetch(sb_info,  base_pegctrlreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegadv, sizeof(m_data32));
    uint32_t m_data32_out_pegadv = m_data32;
    memcpy (&m_data32, data_8_input_pegadv, sizeof(m_data32));
    uint32_t m_data32_in_pegadv = m_data32;
    ASSERT_EQ(m_data32_out_pegadv, m_data32_in_pegadv);
}


/*!
 * @brief Check TargetWriteDebug method.
 */
TEST_F(PegCtrl1, Check_TargetWriteDebug) {
    uint32_t chn_0 = 0x0;

    m_pegctrl->TargetWriteDebug(base_pegctrlreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT);
    m_pegctrl->TargetWriteDebug(base_pegctrlreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x53, 0x01, 0x00, 0x00}, SIZE_32BIT);
    m_pegctrl->TargetWriteDebug(base_pegctrlreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_pegctrl->TargetWriteDebug(base_pegctrlreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_pegctrl->TargetWriteDebug(base_pegctrlreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);

    ASSERT_EQ(0x1U, m_pegctrl->GetPegCtrlReg()->GetPegKceProt()->GetKCE());
    ASSERT_EQ(0x1U, m_pegctrl->GetPegCtrlReg()->GetPegProt(chn_0)->GetGEN());
    ASSERT_EQ(0xFFFFFFFFU, m_pegctrl->GetPegCtrlReg()->GetPegSpid(chn_0)->GetSPID());
    ASSERT_EQ(0xFFFFF000U, m_pegctrl->GetPegCtrlReg()->GetPegBad(chn_0)->GetBAD());
    ASSERT_EQ(0x03FFF000U, m_pegctrl->GetPegCtrlReg()->GetPegAdv(chn_0)->GetADV());

    // Check guard internal value.
    m_pegctrl->GetPegCtrlReg()->GetPegProt(0)->SetGEN(1);
    m_pegctrl->GetPegCtrlReg()->GetPegProt(1)->SetGEN(1);
    m_pegctrl->GetPegCtrlReg()->GetPegProt(2)->SetGEN(1);
    m_pegctrl->GetPegCtrlReg()->GetPegProt(3)->SetGEN(1);
    ASSERT_NO_THROW(m_pegctrl->TargetWriteDebug(base_pegctrlreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT));
    ASSERT_TRUE(m_pegctrl->GetPegCtrlReg()->GetPegInternal()->IsChannelEn(0));
    ASSERT_TRUE(m_pegctrl->GetPegCtrlReg()->GetPegInternal()->IsChannelEn(1));
    ASSERT_TRUE(m_pegctrl->GetPegCtrlReg()->GetPegInternal()->IsChannelEn(2));
    ASSERT_TRUE(m_pegctrl->GetPegCtrlReg()->GetPegInternal()->IsChannelEn(3));
    ASSERT_TRUE(m_pegctrl->GetPegCtrlReg()->GetPegInternal()->IsGuardEn());
}


/*!
 * @brief Check TargetReadDebug method.
 */
TEST_F(PegCtrl1, Check_TargetReadDebug) {
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

    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, data_8_input_pegkcprot, SIZE_32BIT);
    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegprot, SIZE_32BIT);
    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegspid, SIZE_32BIT);
    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegbad, SIZE_32BIT);
    m_pegctrl->TargetWrite(sb_info, base_pegctrlreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0,
                           data_8_input_pegadv, SIZE_32BIT);

    m_pegctrl->TargetReadDebug(base_pegctrlreg + TRegPegCtrlReg::PEGKCPROT_OFFSET, data_8_output_pegkcprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegkcprot, sizeof(m_data32));
    uint32_t m_data32_out_pegkcprot = m_data32;
    // Get KCE key in PEGKCPROT.
    memcpy (&m_data32, data_8_input_pegkcprot, sizeof(m_data32));
    uint32_t m_data32_in_pegkcprot = m_data32 & 0x01;;
    ASSERT_EQ(m_data32_out_pegkcprot, m_data32_in_pegkcprot);

    m_pegctrl->TargetReadDebug(base_pegctrlreg + TRegPegCtrlReg::PEGPROT_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegprot, sizeof(m_data32));
    uint32_t m_data32_out_pegprot = m_data32;
    memcpy (&m_data32, data_8_input_pegprot, sizeof(m_data32));
    uint32_t m_data32_in_pegprot = m_data32;
    ASSERT_EQ(m_data32_out_pegprot, m_data32_in_pegprot);

    m_pegctrl->TargetReadDebug(base_pegctrlreg + TRegPegCtrlReg::PEGSPID_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegspid, sizeof(m_data32));
    uint32_t m_data32_out_pegspid = m_data32;
    memcpy (&m_data32, data_8_input_pegspid, sizeof(m_data32));
    uint32_t m_data32_in_pegspid = m_data32;
    ASSERT_EQ(m_data32_out_pegspid, m_data32_in_pegspid);

    m_pegctrl->TargetReadDebug(base_pegctrlreg + TRegPegCtrlReg::PEGBAD_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegbad, sizeof(m_data32));
    uint32_t m_data32_out_pegbad = m_data32;
    memcpy (&m_data32, data_8_input_pegbad, sizeof(m_data32));
    uint32_t m_data32_in_pegbad = m_data32;
    ASSERT_EQ(m_data32_out_pegbad, m_data32_in_pegbad);

    m_pegctrl->TargetReadDebug(base_pegctrlreg + TRegPegCtrlReg::PEGADV_OFFSET + TRegPegCtrlReg::PEG_CTRL_CHANNEL_RANGE * chn_0, data_8_output_pegadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_pegadv, sizeof(m_data32));
    uint32_t m_data32_out_pegadv = m_data32;
    memcpy (&m_data32, data_8_input_pegadv, sizeof(m_data32));
    uint32_t m_data32_in_pegadv = m_data32;
    ASSERT_EQ(m_data32_out_pegadv, m_data32_in_pegadv);
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
