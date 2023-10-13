/**
 * @file test_crgctrl_20.cpp
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
#include "sideband.h"
#include "crg.h"
#include "mock_sideband.h"
#include "gmock/gmock.h"

// using ::testing::AtLeast;
// using ::testing::_;
// using ::testing::Return;

class CrgCtrl;

// ----------------------------------------
class CrgCtrl2 : public testing::Test {
protected:
    CrgCtrl2() {}
    virtual ~CrgCtrl2() {}
    virtual void SetUp() {
        GuardInternal * guard_internal= new GuardInternal(false, false,
                                                              0x13, 0x8, 0x8, 0x4, 0xA5A5A500, 0x0);
        m_crgctrl           = new CrgCtrl (base_crgctrlreg, base_crgreg_ivc, CORE_G4MH, CORE_VER_20, guard_internal);
    }
    virtual void TearDown() { delete m_crgctrl; }

public:
    CrgCtrl          *m_crgctrl;
    MockSideBand sideband;

public:
    uint32_t m_peid = 0x0;
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

TEST_F(CrgCtrl2, Check_Initial_Setup) {
    ASSERT_NE(nullptr, m_crgctrl->GetCrgCtrlReg());
    ASSERT_FALSE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsGuardEn());
}


/*!
 * @brief Check Reset method.
 */
TEST_F(CrgCtrl2, Check_Reset) {
    SideBand sb_info;
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT);
    ASSERT_EQ(0x1U, m_crgctrl->GetCrgCtrlReg()->GetCrgKceProt()->GetKCE());
    m_crgctrl->HardReset();
    ASSERT_EQ(0x0U, m_crgctrl->GetCrgCtrlReg()->GetCrgKceProt()->GetKCE());
}


/*!
 * @brief Check TargetWrite method.
 */
TEST_F(CrgCtrl2, Check_TargetWrite) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT);

    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x53, 0x01, 0x00, 0x00}, SIZE_32BIT);
    // at least, 1 CRG channel setting is enable.

    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGIVCSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgreg_ivc + TRegCrgCtrlReg::CRGIVCLOCK_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgreg_ivc + TRegCrgCtrlReg::CRGIVCOWNR_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgreg_ivc + TRegCrgCtrlReg::CRGIVCREQ_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);

    ASSERT_EQ(0x1U, m_crgctrl->GetCrgCtrlReg()->GetCrgKceProt()->GetKCE());
    ASSERT_EQ(0x1U, m_crgctrl->GetCrgCtrlReg()->GetCrgProt(chn_0)->GetGEN());
    ASSERT_EQ(0x0U, m_crgctrl->GetCrgCtrlReg()->GetCrgProt(chn_0)->GetOW());
    ASSERT_EQ(0xFFFFFFFFU, m_crgctrl->GetCrgCtrlReg()->GetCrgSpid(chn_0)->GetSPID());
    ASSERT_EQ(0x0007FF00U, m_crgctrl->GetCrgCtrlReg()->GetCrgBad(chn_0)->GetBAD());
    ASSERT_EQ(0x0007FF00U, m_crgctrl->GetCrgCtrlReg()->GetCrgAdv(chn_0)->GetADV());
    ASSERT_EQ(0xFFFFFFFFU, m_crgctrl->GetCrgCtrlReg()->GetCrgIvcSpid(chn_0)->GetIvcSPID());
    ASSERT_EQ(0x1U, m_crgctrl->GetCrgCtrlReg()->GetCrgIvcLock(chn_0)->GetLK());
    ASSERT_EQ(0xFFFFFFFFU, m_crgctrl->GetCrgCtrlReg()->GetCrgIvcOwnr(chn_0)->GetSPID());

    // Check guard internal value.
    m_crgctrl->GetCrgCtrlReg()->GetCrgProt(0)->SetGEN(1);
    m_crgctrl->GetCrgCtrlReg()->GetCrgProt(1)->SetGEN(1);
    m_crgctrl->GetCrgCtrlReg()->GetCrgProt(2)->SetGEN(1);
    m_crgctrl->GetCrgCtrlReg()->GetCrgProt(3)->SetGEN(1);
    m_crgctrl->GetCrgCtrlReg()->GetCrgProt(4)->SetGEN(1);
    m_crgctrl->GetCrgCtrlReg()->GetCrgProt(5)->SetGEN(1);
    m_crgctrl->GetCrgCtrlReg()->GetCrgProt(6)->SetGEN(1);
    m_crgctrl->GetCrgCtrlReg()->GetCrgProt(7)->SetGEN(1);
    ASSERT_NO_THROW(m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsChannelEn(0));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsChannelEn(1));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsChannelEn(2));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsChannelEn(3));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsChannelEn(4));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsChannelEn(5));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsChannelEn(6));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsChannelEn(7));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsGuardEn());
}


/*!
 * @brief Check TargetRead method.
 */
TEST_F(CrgCtrl2, Check_TargetRead) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    uint8_t data_8_input_crgkcprot[4] = {0x01, 0xa5, 0xa5, 0xa5};
    uint8_t data_8_input_crgprot[4] = {0x53, 0x01, 0x01, 0x00};
    uint8_t data_8_input_crgspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_crgbad[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_crgadv[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_crgivcspid[4] = {0xff, 0xff, 0xff, 0xff};

    uint8_t data_8_output_crgkcprot[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgprot[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgbad[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgadv[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgivcspid[4]   = {0xff, 0xff, 0xff, 0xff};

    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, data_8_input_crgkcprot, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgprot, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgspid, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgbad, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgadv, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGIVCSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgivcspid, SIZE_32BIT);

    m_crgctrl->TargetRead(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, data_8_output_crgkcprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgkcprot, sizeof(m_data32));
    uint32_t m_data32_out_crgkcprot = m_data32;
    // Get KCE key in CRGKCPROT.
    memcpy (&m_data32, data_8_input_crgkcprot, sizeof(m_data32));
    uint32_t m_data32_in_crgkcprot = m_data32 & 0x01;;
    ASSERT_EQ(m_data32_out_crgkcprot, m_data32_in_crgkcprot);

    m_crgctrl->TargetRead(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgprot, sizeof(m_data32));
    uint32_t m_data32_out_crgprot = m_data32;
    memcpy (&m_data32, data_8_input_crgprot, sizeof(m_data32));
    uint32_t m_data32_in_crgprot = m_data32;
    ASSERT_EQ(m_data32_out_crgprot, m_data32_in_crgprot);

    m_crgctrl->TargetRead(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgspid, sizeof(m_data32));
    uint32_t m_data32_out_crgspid = m_data32;
    memcpy (&m_data32, data_8_input_crgspid, sizeof(m_data32));
    uint32_t m_data32_in_crgspid = m_data32;
    ASSERT_EQ(m_data32_out_crgspid, m_data32_in_crgspid);

    m_crgctrl->TargetRead(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgbad, sizeof(m_data32));
    uint32_t m_data32_out_crgbad = m_data32;
    memcpy (&m_data32, data_8_input_crgbad, sizeof(m_data32));
    uint32_t m_data32_in_crgbad = m_data32;
    ASSERT_EQ(m_data32_out_crgbad, m_data32_in_crgbad);

    m_crgctrl->TargetRead(sb_info,  base_crgctrlreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgadv, sizeof(m_data32));
    uint32_t m_data32_out_crgadv = m_data32;
    memcpy (&m_data32, data_8_input_crgadv, sizeof(m_data32));
    uint32_t m_data32_in_crgadv = m_data32;
    ASSERT_EQ(m_data32_out_crgadv, m_data32_in_crgadv);

    m_crgctrl->TargetRead(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGIVCSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgivcspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgivcspid, sizeof(m_data32));
    uint32_t m_data32_out_crgivcspid = m_data32;
    memcpy (&m_data32, data_8_input_crgivcspid, sizeof(m_data32));
    uint32_t m_data32_in_crgivcspid = m_data32;
    ASSERT_EQ(m_data32_out_crgivcspid, m_data32_in_crgivcspid);
}


/*!
 * @brief Check TargetFetch method.
 */
TEST_F(CrgCtrl2, Check_TargetFetch) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    uint8_t data_8_input_crgkcprot[4] = {0x01, 0xa5, 0xa5, 0xa5};
    uint8_t data_8_input_crgprot[4] = {0x53, 0x01, 0x00, 0x00};
    uint8_t data_8_input_crgspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_crgbad[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_crgadv[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_crgivcspid[4] = {0xff, 0xff, 0xff, 0xff};

    uint8_t data_8_output_crgkcprot[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgprot[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgbad[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgadv[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgivcspid[4]   = {0xff, 0xff, 0xff, 0xff};

    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, data_8_input_crgkcprot, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgprot, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgspid, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgbad, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgadv, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGIVCSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgivcspid, SIZE_32BIT);

    m_crgctrl->TargetFetch(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, data_8_output_crgkcprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgkcprot, sizeof(m_data32));
    uint32_t m_data32_out_crgkcprot = m_data32;
    // Get KCE key in CRGKCPROT.
    memcpy (&m_data32, data_8_input_crgkcprot, sizeof(m_data32));
    uint32_t m_data32_in_crgkcprot = m_data32 & 0x01;;
    ASSERT_EQ(m_data32_out_crgkcprot, m_data32_in_crgkcprot);

    m_crgctrl->TargetFetch(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgprot, sizeof(m_data32));
    uint32_t m_data32_out_crgprot = m_data32;
    memcpy (&m_data32, data_8_input_crgprot, sizeof(m_data32));
    uint32_t m_data32_in_crgprot = m_data32;
    ASSERT_EQ(m_data32_out_crgprot, m_data32_in_crgprot);

    m_crgctrl->TargetFetch(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgspid, sizeof(m_data32));
    uint32_t m_data32_out_crgspid = m_data32;
    memcpy (&m_data32, data_8_input_crgspid, sizeof(m_data32));
    uint32_t m_data32_in_crgspid = m_data32;
    ASSERT_EQ(m_data32_out_crgspid, m_data32_in_crgspid);

    m_crgctrl->TargetFetch(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgbad, sizeof(m_data32));
    uint32_t m_data32_out_crgbad = m_data32;
    memcpy (&m_data32, data_8_input_crgbad, sizeof(m_data32));
    uint32_t m_data32_in_crgbad = m_data32;
    ASSERT_EQ(m_data32_out_crgbad, m_data32_in_crgbad);

    m_crgctrl->TargetFetch(sb_info,  base_crgctrlreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgadv, sizeof(m_data32));
    uint32_t m_data32_out_crgadv = m_data32;
    memcpy (&m_data32, data_8_input_crgadv, sizeof(m_data32));
    uint32_t m_data32_in_crgadv = m_data32;
    ASSERT_EQ(m_data32_out_crgadv, m_data32_in_crgadv);

    m_crgctrl->TargetFetch(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGIVCSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgivcspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgivcspid, sizeof(m_data32));
    uint32_t m_data32_out_crgivcspid = m_data32;
    memcpy (&m_data32, data_8_input_crgivcspid, sizeof(m_data32));
    uint32_t m_data32_in_crgivcspid = m_data32;
    ASSERT_EQ(m_data32_out_crgivcspid, m_data32_in_crgivcspid);
}


/*!
 * @brief Check TargetWriteDebug method.
 */
TEST_F(CrgCtrl2, Check_TargetWriteDebug) {
    uint32_t chn_0 = 0x1;

    m_crgctrl->TargetWriteDebug(base_crgctrlreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT);
    m_crgctrl->TargetWriteDebug(base_crgctrlreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x53, 0x01, 0x00, 0x00}, SIZE_32BIT);
    m_crgctrl->TargetWriteDebug(base_crgctrlreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crgctrl->TargetWriteDebug(base_crgctrlreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crgctrl->TargetWriteDebug(base_crgctrlreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crgctrl->TargetWriteDebug(base_crgctrlreg + TRegCrgCtrlReg::CRGIVCSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crgctrl->TargetWriteDebug(base_crgreg_ivc + TRegCrgCtrlReg::CRGIVCLOCK_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
//    m_crgctrl->TargetWriteDebug(base_crgreg_ivc + TRegCrgCtrlReg::CRGIVCOWNR_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
//                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
//    m_crgctrl->TargetWriteDebug(base_crgreg_ivc + TRegCrgCtrlReg::CRGIVCREQ_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
//                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);

    ASSERT_EQ(0x1U, m_crgctrl->GetCrgCtrlReg()->GetCrgKceProt()->GetKCE());
    ASSERT_EQ(0x1U, m_crgctrl->GetCrgCtrlReg()->GetCrgProt(chn_0)->GetGEN());
    ASSERT_EQ(0x0U, m_crgctrl->GetCrgCtrlReg()->GetCrgProt(chn_0)->GetOW());
    ASSERT_EQ(0xFFFFFFFFU, m_crgctrl->GetCrgCtrlReg()->GetCrgSpid(chn_0)->GetSPID());
    ASSERT_EQ(0x0007FF00U, m_crgctrl->GetCrgCtrlReg()->GetCrgBad(chn_0)->GetBAD());
    ASSERT_EQ(0x0007FF00U, m_crgctrl->GetCrgCtrlReg()->GetCrgAdv(chn_0)->GetADV());
    ASSERT_EQ(0xFFFFFFFFU, m_crgctrl->GetCrgCtrlReg()->GetCrgIvcSpid(chn_0)->GetIvcSPID());
    ASSERT_EQ(0xFFFFFFFFU, m_crgctrl->GetCrgCtrlReg()->GetCrgIvcOwnr(chn_0)->GetSPID());
    ASSERT_EQ(0x1U, m_crgctrl->GetCrgCtrlReg()->GetCrgIvcLock(chn_0)->GetLK());

    // Check guard internal value.
    m_crgctrl->GetCrgCtrlReg()->GetCrgProt(0)->SetGEN(1);
    m_crgctrl->GetCrgCtrlReg()->GetCrgProt(1)->SetGEN(1);
    m_crgctrl->GetCrgCtrlReg()->GetCrgProt(2)->SetGEN(1);
    m_crgctrl->GetCrgCtrlReg()->GetCrgProt(3)->SetGEN(1);
    m_crgctrl->GetCrgCtrlReg()->GetCrgProt(4)->SetGEN(1);
    m_crgctrl->GetCrgCtrlReg()->GetCrgProt(5)->SetGEN(1);
    m_crgctrl->GetCrgCtrlReg()->GetCrgProt(6)->SetGEN(1);
    m_crgctrl->GetCrgCtrlReg()->GetCrgProt(7)->SetGEN(1);
    ASSERT_NO_THROW(m_crgctrl->TargetWriteDebug(base_crgctrlreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsChannelEn(0));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsChannelEn(1));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsChannelEn(2));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsChannelEn(3));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsChannelEn(4));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsChannelEn(5));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsChannelEn(6));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsChannelEn(7));
    ASSERT_TRUE(m_crgctrl->GetCrgCtrlReg()->GetCrgInternal()->IsGuardEn());
}


/*!
 * @brief Check TargetReadDebug method.
 */
TEST_F(CrgCtrl2, Check_TargetReadDebug) {
    SideBand sb_info;
    uint32_t chn_0 = 0x4;
    uint8_t data_8_input_crgkcprot[4] = {0x01, 0xa5, 0xa5, 0xa5};
    uint8_t data_8_input_crgprot[4] = {0x53, 0x01, 0x00, 0x00};
    uint8_t data_8_input_crgspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_crgbad[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_crgadv[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_crgivcspid[4] = {0xff, 0xff, 0xff, 0xff};

    uint8_t data_8_output_crgkcprot[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgprot[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgbad[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgadv[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgivcspid[4]   = {0xff, 0xff, 0xff, 0xff};

    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, data_8_input_crgkcprot, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgprot, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgspid, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgbad, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgadv, SIZE_32BIT);
    m_crgctrl->TargetWrite(sb_info, base_crgctrlreg + TRegCrgCtrlReg::CRGIVCSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgivcspid, SIZE_32BIT);

    m_crgctrl->TargetReadDebug(base_crgctrlreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, data_8_output_crgkcprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgkcprot, sizeof(m_data32));
    uint32_t m_data32_out_crgkcprot = m_data32;
    // Get KCE key in CRGKCPROT.
    memcpy (&m_data32, data_8_input_crgkcprot, sizeof(m_data32));
    uint32_t m_data32_in_crgkcprot = m_data32 & 0x01;;
    ASSERT_EQ(m_data32_out_crgkcprot, m_data32_in_crgkcprot);

    m_crgctrl->TargetReadDebug(base_crgctrlreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgprot, sizeof(m_data32));
    uint32_t m_data32_out_crgprot = m_data32;
    memcpy (&m_data32, data_8_input_crgprot, sizeof(m_data32));
    uint32_t m_data32_in_crgprot = m_data32;
    ASSERT_EQ(m_data32_out_crgprot, m_data32_in_crgprot);

    m_crgctrl->TargetReadDebug(base_crgctrlreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgspid, sizeof(m_data32));
    uint32_t m_data32_out_crgspid = m_data32;
    memcpy (&m_data32, data_8_input_crgspid, sizeof(m_data32));
    uint32_t m_data32_in_crgspid = m_data32;
    ASSERT_EQ(m_data32_out_crgspid, m_data32_in_crgspid);

    m_crgctrl->TargetReadDebug(base_crgctrlreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgbad, sizeof(m_data32));
    uint32_t m_data32_out_crgbad = m_data32;
    memcpy (&m_data32, data_8_input_crgbad, sizeof(m_data32));
    uint32_t m_data32_in_crgbad = m_data32;
    ASSERT_EQ(m_data32_out_crgbad, m_data32_in_crgbad);

    m_crgctrl->TargetReadDebug(base_crgctrlreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgadv, sizeof(m_data32));
    uint32_t m_data32_out_crgadv = m_data32;
    memcpy (&m_data32, data_8_input_crgadv, sizeof(m_data32));
    uint32_t m_data32_in_crgadv = m_data32;
    ASSERT_EQ(m_data32_out_crgadv, m_data32_in_crgadv);

    m_crgctrl->TargetReadDebug(base_crgctrlreg + TRegCrgCtrlReg::CRGIVCSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgivcspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgivcspid, sizeof(m_data32));
    uint32_t m_data32_out_crgivcspid = m_data32;
    memcpy (&m_data32, data_8_input_crgivcspid, sizeof(m_data32));
    uint32_t m_data32_in_crgivcspid = m_data32;
    ASSERT_EQ(m_data32_out_crgivcspid, m_data32_in_crgivcspid);
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
