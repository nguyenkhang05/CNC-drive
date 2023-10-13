/**
 * @file test_crg_20.cpp
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

class Crg;

static const uint32_t CL0_CRG_UT = 0x0;
static const uint32_t CL1_CRG_UT = 0x1;

// ----------------------------------------
class Crg1 : public testing::Test {
protected:
    Crg1() {}
    virtual ~Crg1() {}
    virtual void SetUp() {
        m_crg           = new Crg (base_crgreg, base_crgreg_ivc, base_crgreg_csg, CORE_G4MH, CORE_VER_20, m_clid);
    }
    virtual void TearDown() { delete m_crg; }

public:
    Crg          *m_crg;
    MockSideBand sideband;

public:
    uint32_t m_clid = CL0_CRG_UT;
    uint32_t base_crgreg = 0xFFC6D000;
    const uint32_t base_crgreg_ivc = 0xFFC6E800;
    const uint32_t base_crgreg_csg = 0xFFC6E000;
    union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
    };
};


/*!
 * @brief Check Initial method.
 */

TEST_F(Crg1, Check_Initial_Setup) {
    ASSERT_NE(nullptr, m_crg->GetCrgGuard());
}


/*!
 * @brief Check Reset method.
 */
TEST_F(Crg1, Check_Reset) {
    SideBand sb_info;
    // Change addr value in CRGADDR to confirm if it is reset or not.
    m_crg->GetCrgCtrl()->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT);
    ASSERT_EQ(0x1U, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgKceProt()->GetKCE());
    m_crg->HardReset();
    ASSERT_EQ(0x0U, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgKceProt()->GetKCE());
}


/*!
 * @brief Check TargetWrite method.
 */
TEST_F(Crg1, Check_TargetWrite) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    ASSERT_THROW(m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGIVCSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_crg->TargetWrite(sb_info, base_crgreg_ivc + TRegCrgCtrlReg::CRGIVCLOCK_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_crg->TargetWrite(sb_info, base_crgreg_ivc + TRegCrgCtrlReg::CRGIVCREQ_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_crg->TargetWrite(sb_info, base_crgreg_ivc + TRegCrgCtrlReg::CRGIVCOWNR_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);

    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT);

    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x53, 0x01, 0x00, 0x00}, SIZE_32BIT);
    // at least, 1 CRG channel setting is enable.

    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGIVCSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg_ivc + TRegCrgCtrlReg::CRGIVCLOCK_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);

    // at least, 1 CSG channel setting is enable.
    m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x53, 0x01, 0x00, 0x00}, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);

    ASSERT_EQ(0x1U, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgKceProt()->GetKCE());
    ASSERT_EQ(0x1U, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgProt(chn_0)->GetGEN());
    ASSERT_EQ(0xFFFFFFFFU, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgSpid(chn_0)->GetSPID());
    ASSERT_EQ(0x0007FF00U, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgBad(chn_0)->GetBAD());
    ASSERT_EQ(0x0007FF00U, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgAdv(chn_0)->GetADV());
    ASSERT_EQ(0xFFFFFFFFU, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgIvcSpid(chn_0)->GetIvcSPID());
    ASSERT_EQ(0xFFFFFFFFU, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgIvcOwnr(chn_0)->GetSPID());
    ASSERT_EQ(0x1U, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgIvcLock(chn_0)->GetLK());

    ASSERT_EQ(0x1U, m_crg->GetCrgSecure()->GetCrgSecureReg()->GetCrgProt(chn_0)->GetGEN());
    ASSERT_EQ(0xFFFFFFFFU, m_crg->GetCrgSecure()->GetCrgSecureReg()->GetCrgSpid(chn_0)->GetSPID());
    ASSERT_EQ(0x0007FF00U, m_crg->GetCrgSecure()->GetCrgSecureReg()->GetCrgBad(chn_0)->GetBAD());
    ASSERT_EQ(0x0007FF00U, m_crg->GetCrgSecure()->GetCrgSecureReg()->GetCrgAdv(chn_0)->GetADV());
}


/*!
 * @brief Check TargetRead method.
 */
TEST_F(Crg1, Check_TargetRead) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    uint8_t data_8_input_crgkcprot[4] = {0x01, 0xa5, 0xa5, 0xa5};
    uint8_t data_8_input_crgprot[4] = {0x53, 0x01, 0x00, 0x00};
    uint8_t data_8_input_crgspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_crgbad[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_crgadv[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_crgivcspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_csgprot[4] = {0x53, 0x01, 0x00, 0x00};
    uint8_t data_8_input_csgspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_csgbad[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_csgadv[4] = {0x00, 0xff, 0x07, 0x00};

    uint8_t data_8_output_crgkcprot[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgprot[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgbad[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgadv[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgivcspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_csgprot[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_csgspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_csgbad[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_csgadv[4]    = {0xff, 0xff, 0xff, 0xff};

    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, data_8_input_crgkcprot, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgprot, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgspid, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgbad, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgadv, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGIVCSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgivcspid, SIZE_32BIT);

    m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_csgprot, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_csgspid, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_csgbad, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_csgadv, SIZE_32BIT);

    m_crg->TargetRead(sb_info, base_crgreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, data_8_output_crgkcprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgkcprot, sizeof(m_data32));
    uint32_t m_data32_out_crgkcprot = m_data32;
    // Get KCE key in CRGKCPROT.
    memcpy (&m_data32, data_8_input_crgkcprot, sizeof(m_data32));
    uint32_t m_data32_in_crgkcprot = m_data32 & 0x01;;
    ASSERT_EQ(m_data32_out_crgkcprot, m_data32_in_crgkcprot);

    m_crg->TargetRead(sb_info, base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgprot, sizeof(m_data32));
    uint32_t m_data32_out_crgprot = m_data32;
    memcpy (&m_data32, data_8_input_crgprot, sizeof(m_data32));
    uint32_t m_data32_in_crgprot = m_data32;
    ASSERT_EQ(m_data32_out_crgprot, m_data32_in_crgprot);

    m_crg->TargetRead(sb_info, base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgspid, sizeof(m_data32));
    uint32_t m_data32_out_crgspid = m_data32;
    memcpy (&m_data32, data_8_input_crgspid, sizeof(m_data32));
    uint32_t m_data32_in_crgspid = m_data32;
    ASSERT_EQ(m_data32_out_crgspid, m_data32_in_crgspid);

    m_crg->TargetRead(sb_info, base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgbad, sizeof(m_data32));
    uint32_t m_data32_out_crgbad = m_data32;
    memcpy (&m_data32, data_8_input_crgbad, sizeof(m_data32));
    uint32_t m_data32_in_crgbad = m_data32;
    ASSERT_EQ(m_data32_out_crgbad, m_data32_in_crgbad);

    m_crg->TargetRead(sb_info,  base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgadv, sizeof(m_data32));
    uint32_t m_data32_out_crgadv = m_data32;
    memcpy (&m_data32, data_8_input_crgadv, sizeof(m_data32));
    uint32_t m_data32_in_crgadv = m_data32;
    ASSERT_EQ(m_data32_out_crgadv, m_data32_in_crgadv);

    m_crg->TargetRead(sb_info, base_crgreg + TRegCrgCtrlReg::CRGIVCSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgivcspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgivcspid, sizeof(m_data32));
    uint32_t m_data32_out_crgivcspid = m_data32;
    memcpy (&m_data32, data_8_input_crgivcspid, sizeof(m_data32));
    uint32_t m_data32_in_crgivcspid = m_data32;
    ASSERT_EQ(m_data32_out_crgivcspid, m_data32_in_crgivcspid);

    m_crg->TargetRead(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_output_csgprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_csgprot, sizeof(m_data32));
    uint32_t m_data32_out_csgprot = m_data32;
    memcpy (&m_data32, data_8_input_csgprot, sizeof(m_data32));
    uint32_t m_data32_in_csgprot = m_data32;
    ASSERT_EQ(m_data32_out_csgprot, m_data32_in_csgprot);

    m_crg->TargetRead(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_output_csgspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_csgspid, sizeof(m_data32));
    uint32_t m_data32_out_csgspid = m_data32;
    memcpy (&m_data32, data_8_input_csgspid, sizeof(m_data32));
    uint32_t m_data32_in_csgspid = m_data32;
    ASSERT_EQ(m_data32_out_csgspid, m_data32_in_csgspid);

    m_crg->TargetRead(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_output_csgbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_csgbad, sizeof(m_data32));
    uint32_t m_data32_out_csgbad = m_data32;
    memcpy (&m_data32, data_8_input_csgbad, sizeof(m_data32));
    uint32_t m_data32_in_csgbad = m_data32;
    ASSERT_EQ(m_data32_out_csgbad, m_data32_in_csgbad);

    m_crg->TargetRead(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_output_csgadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_csgadv, sizeof(m_data32));
    uint32_t m_data32_out_csgadv = m_data32;
    memcpy (&m_data32, data_8_input_csgadv, sizeof(m_data32));
    uint32_t m_data32_in_csgadv = m_data32;
    ASSERT_EQ(m_data32_out_csgadv, m_data32_in_csgadv);
}


/*!
 * @brief Check TargetFetch method.
 */
TEST_F(Crg1, Check_TargetFetch) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    uint8_t data_8_input_crgkcprot[4] = {0x01, 0xa5, 0xa5, 0xa5};
    uint8_t data_8_input_crgprot[4] = {0x53, 0x01, 0x00, 0x00};
    uint8_t data_8_input_crgspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_crgbad[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_crgadv[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_crgivcspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_csgprot[4] = {0x53, 0x01, 0x00, 0x00};
    uint8_t data_8_input_csgspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_csgbad[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_csgadv[4] = {0x00, 0xff, 0x07, 0x00};

    uint8_t data_8_output_crgkcprot[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgprot[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgbad[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgadv[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgivcspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_csgprot[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_csgspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_csgbad[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_csgadv[4]    = {0xff, 0xff, 0xff, 0xff};

    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, data_8_input_crgkcprot, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgprot, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgspid, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgbad, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgadv, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGIVCSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgivcspid, SIZE_32BIT);

    m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_csgprot, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_csgspid, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_csgbad, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_csgadv, SIZE_32BIT);

    m_crg->TargetFetch(sb_info, base_crgreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, data_8_output_crgkcprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgkcprot, sizeof(m_data32));
    uint32_t m_data32_out_crgkcprot = m_data32;
    // Get KCE key in CRGKCPROT.
    memcpy (&m_data32, data_8_input_crgkcprot, sizeof(m_data32));
    uint32_t m_data32_in_crgkcprot = m_data32 & 0x01;;
    ASSERT_EQ(m_data32_out_crgkcprot, m_data32_in_crgkcprot);

    m_crg->TargetFetch(sb_info, base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgprot, sizeof(m_data32));
    uint32_t m_data32_out_crgprot = m_data32;
    memcpy (&m_data32, data_8_input_crgprot, sizeof(m_data32));
    uint32_t m_data32_in_crgprot = m_data32;
    ASSERT_EQ(m_data32_out_crgprot, m_data32_in_crgprot);

    m_crg->TargetFetch(sb_info, base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgspid, sizeof(m_data32));
    uint32_t m_data32_out_crgspid = m_data32;
    memcpy (&m_data32, data_8_input_crgspid, sizeof(m_data32));
    uint32_t m_data32_in_crgspid = m_data32;
    ASSERT_EQ(m_data32_out_crgspid, m_data32_in_crgspid);

    m_crg->TargetFetch(sb_info, base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgbad, sizeof(m_data32));
    uint32_t m_data32_out_crgbad = m_data32;
    memcpy (&m_data32, data_8_input_crgbad, sizeof(m_data32));
    uint32_t m_data32_in_crgbad = m_data32;
    ASSERT_EQ(m_data32_out_crgbad, m_data32_in_crgbad);

    m_crg->TargetFetch(sb_info,  base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgadv, sizeof(m_data32));
    uint32_t m_data32_out_crgadv = m_data32;
    memcpy (&m_data32, data_8_input_crgadv, sizeof(m_data32));
    uint32_t m_data32_in_crgadv = m_data32;
    ASSERT_EQ(m_data32_out_crgadv, m_data32_in_crgadv);

    m_crg->TargetFetch(sb_info, base_crgreg + TRegCrgCtrlReg::CRGIVCSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgivcspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgivcspid, sizeof(m_data32));
    uint32_t m_data32_out_crgivcspid = m_data32;
    memcpy (&m_data32, data_8_input_crgivcspid, sizeof(m_data32));
    uint32_t m_data32_in_crgivcspid = m_data32;
    ASSERT_EQ(m_data32_out_crgivcspid, m_data32_in_crgivcspid);

    m_crg->TargetFetch(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_output_csgprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_csgprot, sizeof(m_data32));
    uint32_t m_data32_out_csgprot = m_data32;
    memcpy (&m_data32, data_8_input_csgprot, sizeof(m_data32));
    uint32_t m_data32_in_csgprot = m_data32;
    ASSERT_EQ(m_data32_out_csgprot, m_data32_in_csgprot);

    m_crg->TargetFetch(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_output_csgspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_csgspid, sizeof(m_data32));
    uint32_t m_data32_out_csgspid = m_data32;
    memcpy (&m_data32, data_8_input_csgspid, sizeof(m_data32));
    uint32_t m_data32_in_csgspid = m_data32;
    ASSERT_EQ(m_data32_out_csgspid, m_data32_in_csgspid);

    m_crg->TargetFetch(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_output_csgbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_csgbad, sizeof(m_data32));
    uint32_t m_data32_out_csgbad = m_data32;
    memcpy (&m_data32, data_8_input_csgbad, sizeof(m_data32));
    uint32_t m_data32_in_csgbad = m_data32;
    ASSERT_EQ(m_data32_out_csgbad, m_data32_in_csgbad);

    m_crg->TargetFetch(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_output_csgadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_csgadv, sizeof(m_data32));
    uint32_t m_data32_out_csgadv = m_data32;
    memcpy (&m_data32, data_8_input_csgadv, sizeof(m_data32));
    uint32_t m_data32_in_csgadv = m_data32;
    ASSERT_EQ(m_data32_out_csgadv, m_data32_in_csgadv);
}


/*!
 * @brief Check TargetWriteDebug method.
 */
TEST_F(Crg1, Check_TargetWriteDebug) {
    uint32_t chn_0 = 0x0;

    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x53, 0x01, 0x00, 0x00}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGIVCSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg_ivc + TRegCrgCtrlReg::CRGIVCLOCK_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x53, 0x01, 0x00, 0x00}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);

    ASSERT_EQ(0x1U, m_crg->GetCrgSecure()->GetCrgSecureReg()->GetCrgProt(chn_0)->GetGEN());
    ASSERT_EQ(0xFFFFFFFFU, m_crg->GetCrgSecure()->GetCrgSecureReg()->GetCrgSpid(chn_0)->GetSPID());
    ASSERT_EQ(0x0007FF00U, m_crg->GetCrgSecure()->GetCrgSecureReg()->GetCrgBad(chn_0)->GetBAD());
    ASSERT_EQ(0x0007FF00U, m_crg->GetCrgSecure()->GetCrgSecureReg()->GetCrgAdv(chn_0)->GetADV());

    ASSERT_EQ(0x1U, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgKceProt()->GetKCE());
    ASSERT_EQ(0x1U, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgProt(chn_0)->GetGEN());
    ASSERT_EQ(0xFFFFFFFFU, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgSpid(chn_0)->GetSPID());
    ASSERT_EQ(0x0007FF00U, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgBad(chn_0)->GetBAD());
    ASSERT_EQ(0x0007FF00U, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgAdv(chn_0)->GetADV());
    ASSERT_EQ(0xFFFFFFFFU, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgIvcSpid(chn_0)->GetIvcSPID());
    ASSERT_EQ(0xFFFFFFFFU, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgIvcOwnr(chn_0)->GetSPID());
    ASSERT_EQ(0x1U, m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgIvcLock(chn_0)->GetLK());
}


/*!
 * @brief Check TargetReadDebug method.
 */
TEST_F(Crg1, Check_TargetReadDebug) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    uint8_t data_8_input_crgkcprot[4] = {0x01, 0xa5, 0xa5, 0xa5};
    uint8_t data_8_input_crgprot[4] = {0x53, 0x01, 0x00, 0x00};
    uint8_t data_8_input_crgspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_crgbad[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_crgadv[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_crgivcspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_csgprot[4] = {0x53, 0x01, 0x00, 0x00};
    uint8_t data_8_input_csgspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_csgbad[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_csgadv[4] = {0x00, 0xff, 0x07, 0x00};

    uint8_t data_8_output_crgkcprot[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgprot[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgbad[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgadv[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgivcspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_csgprot[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_csgspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_csgbad[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_csgadv[4]    = {0xff, 0xff, 0xff, 0xff};

    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, data_8_input_crgkcprot, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgprot, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgspid, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgbad, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgadv, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg + TRegCrgCtrlReg::CRGIVCSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                           data_8_input_crgivcspid, SIZE_32BIT);

    m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_csgprot, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_csgspid, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_csgbad, SIZE_32BIT);
    m_crg->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_csgadv, SIZE_32BIT);

    m_crg->TargetReadDebug(base_crgreg + TRegCrgCtrlReg::CRGKCPROT_OFFSET, data_8_output_crgkcprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgkcprot, sizeof(m_data32));
    uint32_t m_data32_out_crgkcprot = m_data32;
    // Get KCE key in CRGKCPROT.
    memcpy (&m_data32, data_8_input_crgkcprot, sizeof(m_data32));
    uint32_t m_data32_in_crgkcprot = m_data32 & 0x01;;
    ASSERT_EQ(m_data32_out_crgkcprot, m_data32_in_crgkcprot);

    m_crg->TargetReadDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgprot, sizeof(m_data32));
    uint32_t m_data32_out_crgprot = m_data32;
    memcpy (&m_data32, data_8_input_crgprot, sizeof(m_data32));
    uint32_t m_data32_in_crgprot = m_data32;
    ASSERT_EQ(m_data32_out_crgprot, m_data32_in_crgprot);

    m_crg->TargetReadDebug(base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgspid, sizeof(m_data32));
    uint32_t m_data32_out_crgspid = m_data32;
    memcpy (&m_data32, data_8_input_crgspid, sizeof(m_data32));
    uint32_t m_data32_in_crgspid = m_data32;
    ASSERT_EQ(m_data32_out_crgspid, m_data32_in_crgspid);

    m_crg->TargetReadDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgbad, sizeof(m_data32));
    uint32_t m_data32_out_crgbad = m_data32;
    memcpy (&m_data32, data_8_input_crgbad, sizeof(m_data32));
    uint32_t m_data32_in_crgbad = m_data32;
    ASSERT_EQ(m_data32_out_crgbad, m_data32_in_crgbad);

    m_crg->TargetReadDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgadv, sizeof(m_data32));
    uint32_t m_data32_out_crgadv = m_data32;
    memcpy (&m_data32, data_8_input_crgadv, sizeof(m_data32));
    uint32_t m_data32_in_crgadv = m_data32;
    ASSERT_EQ(m_data32_out_crgadv, m_data32_in_crgadv);

    m_crg->TargetReadDebug(base_crgreg + TRegCrgCtrlReg::CRGIVCSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0, data_8_output_crgivcspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgivcspid, sizeof(m_data32));
    uint32_t m_data32_out_crgivcspid = m_data32;
    memcpy (&m_data32, data_8_input_crgivcspid, sizeof(m_data32));
    uint32_t m_data32_in_crgivcspid = m_data32;
    ASSERT_EQ(m_data32_out_crgivcspid, m_data32_in_crgivcspid);

    m_crg->TargetReadDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_output_csgprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_csgprot, sizeof(m_data32));
    uint32_t m_data32_out_csgprot = m_data32;
    memcpy (&m_data32, data_8_input_csgprot, sizeof(m_data32));
    uint32_t m_data32_in_csgprot = m_data32;
    ASSERT_EQ(m_data32_out_csgprot, m_data32_in_csgprot);

    m_crg->TargetReadDebug(base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_output_csgspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_csgspid, sizeof(m_data32));
    uint32_t m_data32_out_csgspid = m_data32;
    memcpy (&m_data32, data_8_input_csgspid, sizeof(m_data32));
    uint32_t m_data32_in_csgspid = m_data32;
    ASSERT_EQ(m_data32_out_csgspid, m_data32_in_csgspid);

    m_crg->TargetReadDebug(base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_output_csgbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_csgbad, sizeof(m_data32));
    uint32_t m_data32_out_csgbad = m_data32;
    memcpy (&m_data32, data_8_input_csgbad, sizeof(m_data32));
    uint32_t m_data32_in_csgbad = m_data32;
    ASSERT_EQ(m_data32_out_csgbad, m_data32_in_csgbad);

    m_crg->TargetReadDebug(base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_output_csgadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_csgadv, sizeof(m_data32));
    uint32_t m_data32_out_csgadv = m_data32;
    memcpy (&m_data32, data_8_input_csgadv, sizeof(m_data32));
    uint32_t m_data32_in_csgadv = m_data32;
    ASSERT_EQ(m_data32_out_csgadv, m_data32_in_csgadv);
}


// ----------------------------------------
class Crg2 : public testing::Test {
protected:
    Crg2() {}
    virtual ~Crg2() {}
    virtual void SetUp() {
        m_crg           = new Crg (base_crgreg, base_crgreg_ivc, base_crgreg_csg, CORE_G4MH, CORE_VER_20, m_clid);
    }
    virtual void TearDown() { delete m_crg; }

public:
    Crg          *m_crg;
    MockSideBand sideband;

public:
    uint32_t m_clid = CL0_CRG_UT;
    uint32_t base_crgreg = 0xFFC6D000;
    const uint32_t base_crgreg_ivc = 0xFFC6E800;
    const uint32_t base_crgreg_csg = 0xFFC6E000;
};


/*!
 * @brief Check IsBank0Addr method.
 * Bank0: 0x0, 0x8, Bank1: 0x4, 0xC.
 */

TEST_F(Crg2, Check_IsBank0Addr_1) {
    ASSERT_TRUE(m_crg->IsBank0Addr(0x0));
    ASSERT_TRUE(m_crg->IsBank0Addr(0x8));

}


TEST_F(Crg2, Check_IsBank0Addr_2) {
    ASSERT_FALSE(m_crg->IsBank0Addr(0x4));
    ASSERT_FALSE(m_crg->IsBank0Addr(0xc));

}


/*!
 * @brief Check IsAddressMatch method.
 * This function is only used to check in area [0xFC000000, 0xFFFFFFFF].
 */

TEST_F(Crg2, Check_IsAddressMatch_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Set BAD = 0xFC00_0000, ADV = 0x0000_0000.
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsAddressMatch(chn_0, 0xFC000000, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsAddressMatch(chn_0, 0xFC000004, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsAddressMatch(chn_0, 0xFC000FFF, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsAddressMatch(chn_0, 0xFC00FFFF, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsAddressMatch(chn_0, 0xFC0FFFFF, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsAddressMatch(chn_0, 0xFCFFFFFF, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsAddressMatch(chn_0, 0xFFFFFFFF, CORE_G4MH, CORE_VER_20));

    // Set ADV = 0x03FFF000,
    m_crg->GetCrgCtrl()->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);
    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsAddressMatch(chn_0, 0xFC000000, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsAddressMatch(chn_0, 0xFC000004, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsAddressMatch(chn_0, 0xFC000FFF, CORE_G4MH, CORE_VER_20));
}


TEST_F(Crg2, Check_IsAddressMatch_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Set BAD = 0xFC00_0000, ADV = 0x0000_0000.
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);

    // Set ADV = 0x03FFF000,
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);

    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsAddressMatch(chn_0, 0xFC00FFFF, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsAddressMatch(chn_0, 0xFC0FFFFF, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsAddressMatch(chn_0, 0xFCFFFFFF, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsAddressMatch(chn_0, 0xFFFFFFFF, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsAddressMatch(chn_0, 0xFFFFFFFF, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsSpidAllowed method.
 */

TEST_F(Crg2, Check_IsSpidAllowed_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable SPID
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetSPID(0);
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
    sb_info.SetSPID(31);
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
    sb_info.SetSPID(15);
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));

}


TEST_F(Crg2, Check_IsSpidAllowed_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // Enable SPID
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x01, 0x80, 0x00, 0x80}, SIZE_32BIT);
    sb_info.SetSPID(0);
    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
    sb_info.SetSPID(31);
    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
    sb_info.SetSPID(15);
    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsWriteGloAllowed method.
 */

TEST_F(Crg2, Check_IsWriteGloAllowed_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable WG
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);

    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsWriteGloAllowed(chn_0, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsWriteGloAllowed(chn_0, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsWriteGloAllowed(chn_0, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsWriteGloAllowed(chn_0, DATA_RMW, CORE_G4MH, CORE_VER_20));
}


TEST_F(Crg2, Check_IsWriteGloAllowed_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // Enable WG
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x02, 0x00, 0x00, 0x00}, SIZE_32BIT);
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsWriteGloAllowed(chn_0, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsWriteGloAllowed(chn_0, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsWriteGloAllowed(chn_0, DATA_RMW, CORE_G4MH, CORE_VER_20));
}


TEST_F(Crg2, Check_IsWriteGloAllowed_3) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // Enable WG
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x02, 0x00, 0x00, 0x00}, SIZE_32BIT);

    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsWriteGloAllowed(chn_0, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}



/*!
 * @brief Check IsReadGloAllowed method.
 */

TEST_F(Crg2, Check_IsReadGloAllowed_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable RG
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);

    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsReadGloAllowed(chn_0, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsReadGloAllowed(chn_0, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsReadGloAllowed(chn_0, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsReadGloAllowed(chn_0, DATA_RMW, CORE_G4MH, CORE_VER_20));
}


TEST_F(Crg2, Check_IsReadGloAllowed_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Enable RG
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x01, 0x00, 0x00, 0x00}, SIZE_32BIT);

    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsReadGloAllowed(chn_0, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsReadGloAllowed(chn_0, DATA_RMW, CORE_G4MH, CORE_VER_20));
}


TEST_F(Crg2, Check_IsReadGloAllowed_3) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // Enable RG
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x01, 0x00, 0x00, 0x00}, SIZE_32BIT);

    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsReadGloAllowed(chn_0, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsReadGloAllowed(chn_0, INST_READ, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsUMDisabled method.
 */

TEST_F(Crg2, Check_IsUMDisabled_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable UME
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetUM(0);
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsUMDisabled(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
    sb_info.SetUM(1);
    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsUMDisabled(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
}


TEST_F(Crg2, Check_IsUMDisabled_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // Enable UME
    m_crg->GetCrgCtrl()->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x10, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetUM(0);
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsUMDisabled(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
}


TEST_F(Crg2, Check_IsUMDisabled_3) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // Enable UME
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x10, 0x00, 0x00, 0x00}, SIZE_32BIT);

    sb_info.SetUM(1);
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsUMDisabled(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsDbgAllowed method.
 */

TEST_F(Crg2, Check_IsDbgAllowed_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable DB permission
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetDM(0);
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsDbgAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
    sb_info.SetDM(1);
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsDbgAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
}


TEST_F(Crg2, Check_IsDbgAllowed_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // Enable DB permission
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x40, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetDM(0);
    ASSERT_FALSE(m_crg->GetCrgCtrl()->IsDbgAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
}


TEST_F(Crg2, Check_IsDbgAllowed_3) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // Enable DB permission
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x40, 0x00, 0x00, 0x00}, SIZE_32BIT);

    sb_info.SetDM(1);
    ASSERT_TRUE(m_crg->GetCrgCtrl()->IsDbgAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
}


// ----------------------------------------
// Crg3 is used to checked IsMemAccPermit in case PE of memory access is PE contains CRG.

class Crg3 : public testing::Test {
protected:
    Crg3() {}
    virtual ~Crg3() {}
    virtual void SetUp() {
        m_crg           = new Crg (base_crgreg, base_crgreg_ivc, base_crgreg_csg, CORE_G4MH, CORE_VER_20, m_clid);
    }
    virtual void TearDown() { delete m_crg; }

public:
    Crg          *m_crg;
    MockSideBand sideband;

public:
    uint32_t m_clid = CL0_CRG_UT;
    const uint32_t base_crgreg = 0xFFC6D000;
    const uint32_t base_crgreg_ivc = 0xFFC6E800;
    const uint32_t base_crgreg_csg = 0xFFC6E000;
};


/*!
 * @brief Check IsMemAccPermit method.
 * case 1: Guard Setting is not enable.
 * Return SYSERR_NONE
 */

TEST_F(Crg3, Check_IsMemAccPermit_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(false);
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 2: In case read type, RMW instruction (include NOT1/CLR1/SET1/CAXI) is ignored
 * Return SYSERR_NONE
 */

TEST_F(Crg3, Check_IsMemAccPermit_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    sb_info.SetBitOp(SideBand::BITOP_SET);
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    sb_info.SetBitOp(SideBand::BITOP_CLR);
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    sb_info.SetBitOp(SideBand::BITOP_NOT);
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    sb_info.SetBitOp(SideBand::BITOP_CAXI);
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
}


// ----------------------------------------
class Crg4 : public testing::Test {
protected:
    Crg4() {}
    virtual ~Crg4() {}
    virtual void SetUp() {
        m_crg           = new Crg (base_crgreg, base_crgreg_ivc, base_crgreg_csg, CORE_G4MH, CORE_VER_20, m_clid);
    }
    virtual void TearDown() { delete m_crg; }

public:
    Crg          *m_crg;
    MockSideBand sideband;

public:
    uint32_t m_clid = CL1_CRG_UT;
    const uint32_t base_crgreg = 0xFFC6D000;
    const uint32_t base_crgreg_ivc = 0xFFC6E800;
    const uint32_t base_crgreg_csg = 0xFFC6E000;
};


/*!
 * @brief Check IsMemAccPermit method.
 * case 4.0: Address at channel 0 is not match.
 * Return SYSERR_SEG
 */

TEST_F(Crg4, Check_IsMemAccPermit_AddrMatch_Sec_ch0) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    m_crg->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x40, 0x00, 0x00, 0x00}, SIZE_32BIT);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgSecure()->GetCrgSecureReg()->GetCrgProt(chn_0)->SetGEN(true);
    ASSERT_TRUE(m_crg->GetCrgSecure()->GetCrgSecureReg()->GetCrgProt(chn_0)->GetGEN());
    sb_info.SetBitOp(SideBand::BITOP_NONE);
    sb_info.SetDM(1);
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 4.1: Address at channel 0 is not match.
 * Return SYSERR_SEG
 */

TEST_F(Crg4, Check_IsMemAccPermit_AddrMatch_ch0) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_0, true);
    m_crg->GetCrgSecure()->GetCrgSecureReg()->GetCrgInternal()->SetChannelEn(chn_0, true);
    sb_info.SetBitOp(SideBand::BITOP_NONE);
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 4.2: Address at channel 1 is not match.
 * Return SYSERR_SEG
 */

TEST_F(Crg4, Check_IsMemAccPermit_AddrMatch_ch1) {
    SideBand sb_info;
    uint32_t chn_1 = 0x1;
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_1, true);
    sb_info.SetBitOp(SideBand::BITOP_NONE);
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 4.3: Address at channel 2 is not match.
 * Return SYSERR_SEG
 */

TEST_F(Crg4, Check_IsMemAccPermit_AddrMatch_ch2) {
    SideBand sb_info;
    uint32_t chn_2 = 0x2;
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_2, true);
    sb_info.SetBitOp(SideBand::BITOP_NONE);
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 4.4: Address at channel 3 is not match.
 * Return SYSERR_SEG
 */

TEST_F(Crg4, Check_IsMemAccPermit_AddrMatch_ch3) {
    SideBand sb_info;
    uint32_t chn_3 = 0x3;
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_3, true);
    sb_info.SetBitOp(SideBand::BITOP_NONE);
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 5.1: DB permission at channel 0 is match.
 * Return SYSERR_NONE
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsDbgAllowed_ch0) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);

    // Enable Debug permision
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x40, 0x00, 0x00, 0x00}, SIZE_32BIT);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_0, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(1);

    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 5.2: DB permission at channel 1 is match.
 * Return SYSERR_NONE
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsDbgAllowed_ch1) {
    SideBand sb_info;
    uint32_t chn_1 = 0x1;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);

    // Enable Debug permision
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x40, 0x00, 0x00, 0x00}, SIZE_32BIT);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_1, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(1);

    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 5.3: DB permission at channel 2 is match.
 * Return SYSERR_NONE
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsDbgAllowed_ch2) {
    SideBand sb_info;
    uint32_t chn_2 = 0x2;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);

    // Enable Debug permision
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x40, 0x00, 0x00, 0x00}, SIZE_32BIT);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_2, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(1);

    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 5.4: DB permission at channel 3 is match.
 * Return SYSERR_NONE
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsDbgAllowed_ch3) {
    SideBand sb_info;
    uint32_t chn_3 = 0x3;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);

    // Enable Debug permision
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x40, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_3, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(1);

    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 6.1: UM permission at channel 0 is not match.
 * Return SYSERR_SEG
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsUMAllowed_ch0) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_0, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // UM is disable.
    sb_info.SetUM(1);
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 6.2: UM permission at channel 1 is not match.
 * Return SYSERR_SEG
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsUMAllowed_ch1) {
    SideBand sb_info;
    uint32_t chn_1 = 0x1;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x00, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_1, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // UM is disable.
    sb_info.SetUM(1);
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 6.3: UM permission at channel 2 is not match.
 * Return SYSERR_SEG
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsUMAllowed_ch2) {
    SideBand sb_info;
    uint32_t chn_2 = 0x2;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x00, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_2, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // UM is disable.
    sb_info.SetUM(1);
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 6.4: UM permission at channel 3 is not match.
 * Return SYSERR_SEG
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsUMAllowed_ch3) {
    SideBand sb_info;
    uint32_t chn_3 = 0x3;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x00, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_3, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // UM is disable.
    sb_info.SetUM(1);
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 7.1: SPID at channel 0 is match.
 * Return SYSERR_NONE
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsSPIDllowed_ch0) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Enable SPID
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x02, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_0, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 8.1: WG at channel 0 is match.
 * Return SYSERR_NONE
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsWriteGloAllowed_ch0) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable SPID
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x02, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_0, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));

    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));

    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 9.1: RG at channel 0 is match.
 * Return SYSERR_NONE
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsReadGloAllowed_ch0) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable SPID
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_0,
                                    new uint8_t[4] {0x01, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_0, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));

    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));

    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 7.2: SPID at channel 1 is match.
 * Return SYSERR_NONE
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsSPIDllowed_ch1) {
    SideBand sb_info;
    uint32_t chn_1 = 0x1;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Enable SPID
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x02, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x00, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_1, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 8.2: WG at channel 1 is match.
 * Return SYSERR_NONE
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsWriteGloAllowed_ch1) {
    SideBand sb_info;
    uint32_t chn_1 = 0x1;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable SPID
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x02, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_1, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));

    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));

    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 9.2: RG at channel 1 is match.
 * Return SYSERR_NONE
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsReadGloAllowed_ch1) {
    SideBand sb_info;
    uint32_t chn_1 = 0x1;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable SPID
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_1,
                                    new uint8_t[4] {0x01, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_1, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));

    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));

    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 7.3: SPID at channel 2 is match.
 * Return SYSERR_NONE
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsSPIDllowed_ch2) {
    SideBand sb_info;
    uint32_t chn_2 = 0x2;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Enable SPID
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x02, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x00, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_2, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 8.3: WG at channel 2 is match.
 * Return SYSERR_NONE
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsWriteGloAllowed_ch2) {
    SideBand sb_info;
    uint32_t chn_2 = 0x2;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable SPID
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x02, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_2, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));

    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));

    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 9.3: RG at channel 2 is match.
 * Return SYSERR_NONE
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsReadGloAllowed_ch2) {
    SideBand sb_info;
    uint32_t chn_2 = 0x2;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable SPID
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_2,
                                    new uint8_t[4] {0x01, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_2, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));

    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));

    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 7.4: SPID at channel 3 is match.
 * Return SYSERR_NONE
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsSPIDllowed_ch3) {
    SideBand sb_info;
    uint32_t chn_3 = 0x3;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Enable SPID
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x02, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x00, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_3, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 8.4: WG at channel 3 is match.
 * Return SYSERR_NONE
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsWriteGloAllowed_ch3) {
    SideBand sb_info;
    uint32_t chn_3 = 0x3;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable SPID
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x02, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_3, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));

    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));

    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 9.4: RG at channel 3 is match.
 * Return SYSERR_NONE
 */

TEST_F(Crg4, Check_IsMemAccPermit_IsReadGloAllowed_ch3) {
    SideBand sb_info;
    uint32_t chn_3 = 0x3;

    // BAD, ADV is satisfied
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGBAD_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGADV_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable SPID
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGSPID_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    // Disable Debug permision
    // Set WG, RG to invalid access.
    m_crg->TargetWriteDebug(base_crgreg + TRegCrgCtrlReg::CRGPROT_OFFSET + TRegCrgCtrlReg::CRG_CTRL_CHANNEL_RANGE_G4MH2X * chn_3,
                                    new uint8_t[4] {0x01, 0x01, 0x00, 0x00}, SIZE_32BIT);

    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->Reset();
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetGuardEn(true);
    m_crg->GetCrgCtrl()->GetCrgCtrlReg()->GetCrgInternal()->SetChannelEn(chn_3, true);

    sb_info.SetBitOp(SideBand::BITOP_NONE);

    sb_info.SetDM(0);

    // Access is not in UM mode.
    sb_info.SetUM(0);

    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_CRG, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20));

    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20));

    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFFFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC00FFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_EQ(SYSERR_NONE, m_crg->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20));
}


// ----------------------------------------
class Crg5 : public testing::Test {
protected:
    Crg5() {}
    virtual ~Crg5() {}
    virtual void SetUp() {
        m_crg           = new Crg (base_crgreg, base_crgreg_ivc, base_crgreg_csg, CORE_G4MH, CORE_VER_20, m_clid);
    }
    virtual void TearDown() { delete m_crg; }

public:
    Crg          *m_crg;
    MockSideBand sideband;

public:
    uint32_t m_clid = CL0_CRG_UT;
    uint32_t base_crgreg = 0xFFC6D000;
    uint32_t base_crgreg_ivc = 0x0;
    uint32_t base_crgreg_csg = 0x0;
};


/*!
 * @brief Check CauseError method.
 * case 1:
 * Return SYSERR_CRG
 */

TEST_F(Crg5, Check_CauseError_SX2MBLCL) {
    uint32_t err_dm = 0x1, err_um = 0x1, err_spid = 0xa, err_spid_2 = 0x7;
    uint32_t err_addr_bank0 = 0xFFFFFFF0;
    SideBand sb_info;
    sb_info.SetBMID(0x0);
    sb_info.SetSPID(err_spid);
    sb_info.SetDM(err_dm);
    sb_info.SetUM(err_um);

    ASSERT_EQ(SYSERR_CRG, m_crg->CauseError(sb_info, err_addr_bank0,
                           INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_NE(nullptr, m_crg->GetCrgErrCap());

    // First error
    ASSERT_EQ(0xFFC6D930, m_crg->GetCrgErrCap()->m_reg_addr_base);

    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSEC());
    ASSERT_EQ(0x400U, m_crg->GetCrgErrCap()->GetCrgSpidErrStat()->GetSPIDERR());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_crg->GetCrgErrCap()->GetCrgErrType()->GetDBG());
    ASSERT_EQ(err_um, m_crg->GetCrgErrCap()->GetCrgErrType()->GetUM());
    ASSERT_EQ(err_addr_bank0, m_crg->GetCrgErrCap()->GetCrgErrAddr()->GetAddr());

    sb_info.SetSPID(err_spid_2);

    ASSERT_EQ(SYSERR_CRG, m_crg->CauseError(sb_info, err_addr_bank0,
                           INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_NE(nullptr, m_crg->GetCrgErrCap());
    ASSERT_EQ(0xFFC6D930, m_crg->GetCrgErrCap()->m_reg_addr_base);

    // Second error: Update OVF
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSEC());
    ASSERT_EQ(0x480U, m_crg->GetCrgErrCap()->GetCrgSpidErrStat()->GetSPIDERR());
    ASSERT_EQ(0x1U, m_crg->GetCrgErrCap()->GetCrgErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_crg->GetCrgErrCap()->GetCrgErrType()->GetDBG());
    ASSERT_EQ(err_um, m_crg->GetCrgErrCap()->GetCrgErrType()->GetUM());
    ASSERT_EQ(err_addr_bank0, m_crg->GetCrgErrCap()->GetCrgErrAddr()->GetAddr());
}


/*!
 * @brief Check CauseError method.
 * case 2:
 * Return SYSERR_CRG
 */

TEST_F(Crg5, Check_CauseError_SX2MBHCL) {
    uint32_t err_dm = 0x1, err_um = 0x1, err_spid = 0xa, err_spid_2 = 0x0;
    uint32_t err_addr_bank1 = 0xcafecafe;
    SideBand sb_info;
    sb_info.SetBMID(0x0);
    sb_info.SetSPID(err_spid);
    sb_info.SetDM(err_dm);
    sb_info.SetUM(err_um);

    ASSERT_EQ(SYSERR_CRG, m_crg->CauseError(sb_info, err_addr_bank1,
                           INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_NE(nullptr, m_crg->GetCrgErrCap());

    // First error
    ASSERT_EQ(0xFFC6D920, m_crg->GetCrgErrCap()->m_reg_addr_base);

    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSEC());
    ASSERT_EQ(0x400U, m_crg->GetCrgErrCap()->GetCrgSpidErrStat()->GetSPIDERR());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_crg->GetCrgErrCap()->GetCrgErrType()->GetDBG());
    ASSERT_EQ(err_um, m_crg->GetCrgErrCap()->GetCrgErrType()->GetUM());
    ASSERT_EQ(err_addr_bank1, m_crg->GetCrgErrCap()->GetCrgErrAddr()->GetAddr());

    sb_info.SetSPID(err_spid_2);

    ASSERT_EQ(SYSERR_CRG, m_crg->CauseError(sb_info, err_addr_bank1,
                           INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_NE(nullptr, m_crg->GetCrgErrCap());
    ASSERT_EQ(0xFFC6D920, m_crg->GetCrgErrCap()->m_reg_addr_base);

    // Second error: Update OVF
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSEC());
    ASSERT_EQ(0x401U, m_crg->GetCrgErrCap()->GetCrgSpidErrStat()->GetSPIDERR());
    ASSERT_EQ(0x1U, m_crg->GetCrgErrCap()->GetCrgErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_crg->GetCrgErrCap()->GetCrgErrType()->GetDBG());
    ASSERT_EQ(err_um, m_crg->GetCrgErrCap()->GetCrgErrType()->GetUM());
    ASSERT_EQ(err_addr_bank1, m_crg->GetCrgErrCap()->GetCrgErrAddr()->GetAddr());
}


/*!
 * @brief Check CauseError method.
 * case 3:
 * Return SYSERR_CRG
 */

TEST_F(Crg5, Check_CauseError_CRAMLCLm) {
    uint32_t err_dm = 0x1, err_um = 0x1, err_spid = 0xa, err_spid_2 = 0x4;
    uint32_t err_addr_bank0 = 0xFFFFFFF0;
    SideBand sb_info;
    sb_info.SetBMID(0x0);
    sb_info.SetSPID(err_spid);
    sb_info.SetDM(err_dm);
    sb_info.SetUM(err_um);

    ASSERT_EQ(SYSERR_CRG, m_crg->CauseError(sb_info, err_addr_bank0,
                           DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_NE(nullptr, m_crg->GetCrgErrCap());

    // First error
    ASSERT_EQ(0xFFC6D910, m_crg->GetCrgErrCap()->m_reg_addr_base);

    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSEC());
    ASSERT_EQ(0x400U, m_crg->GetCrgErrCap()->GetCrgSpidErrStat()->GetSPIDERR());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrStat()->GetOVF());
    ASSERT_EQ(0x1U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_crg->GetCrgErrCap()->GetCrgErrType()->GetDBG());
    ASSERT_EQ(err_um, m_crg->GetCrgErrCap()->GetCrgErrType()->GetUM());
    ASSERT_EQ(err_addr_bank0, m_crg->GetCrgErrCap()->GetCrgErrAddr()->GetAddr());

    sb_info.SetSPID(err_spid_2);

    ASSERT_EQ(SYSERR_CRG, m_crg->CauseError(sb_info, err_addr_bank0,
                           DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_NE(nullptr, m_crg->GetCrgErrCap());
    ASSERT_EQ(0xFFC6D910, m_crg->GetCrgErrCap()->m_reg_addr_base);

    // Second error: Update OVF
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSEC());
    ASSERT_EQ(0x410U, m_crg->GetCrgErrCap()->GetCrgSpidErrStat()->GetSPIDERR());
    ASSERT_EQ(0x1U, m_crg->GetCrgErrCap()->GetCrgErrStat()->GetOVF());
    ASSERT_EQ(0x1U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_crg->GetCrgErrCap()->GetCrgErrType()->GetDBG());
    ASSERT_EQ(err_um, m_crg->GetCrgErrCap()->GetCrgErrType()->GetUM());
    ASSERT_EQ(err_addr_bank0, m_crg->GetCrgErrCap()->GetCrgErrAddr()->GetAddr());
}


/*!
 * @brief Check CauseError method.
 * case 4:
 * Return SYSERR_CRG
 */

TEST_F(Crg5, Check_CauseError_CRAMHCLm) {
    uint32_t err_dm = 0x1, err_um = 0x1, err_spid = 0xa, err_spid_2 = 0xb;
    uint32_t err_addr_bank1 = 0xcafecafe;
    SideBand sb_info;
    sb_info.SetBMID(0x0);
    sb_info.SetSPID(err_spid);
    sb_info.SetDM(err_dm);
    sb_info.SetUM(err_um);

    ASSERT_EQ(SYSERR_CRG, m_crg->CauseError(sb_info, err_addr_bank1,
                          DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_NE(nullptr, m_crg->GetCrgErrCap());

    // First error
    ASSERT_EQ(0xFFC6D900, m_crg->GetCrgErrCap()->m_reg_addr_base);

    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSEC());
    ASSERT_EQ(0x400U, m_crg->GetCrgErrCap()->GetCrgSpidErrStat()->GetSPIDERR());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrStat()->GetOVF());
    ASSERT_EQ(0x1U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_crg->GetCrgErrCap()->GetCrgErrType()->GetDBG());
    ASSERT_EQ(err_um, m_crg->GetCrgErrCap()->GetCrgErrType()->GetUM());
    ASSERT_EQ(err_addr_bank1, m_crg->GetCrgErrCap()->GetCrgErrAddr()->GetAddr());

    sb_info.SetSPID(err_spid_2);

    ASSERT_EQ(SYSERR_CRG, m_crg->CauseError(sb_info, err_addr_bank1,
                         DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_NE(nullptr, m_crg->GetCrgErrCap());
    ASSERT_EQ(0xFFC6D900, m_crg->GetCrgErrCap()->m_reg_addr_base);

    // Second error: Update OVF
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSEC());
    ASSERT_EQ(0xc00U, m_crg->GetCrgErrCap()->GetCrgSpidErrStat()->GetSPIDERR());
    ASSERT_EQ(0x1U, m_crg->GetCrgErrCap()->GetCrgErrStat()->GetOVF());
    ASSERT_EQ(0x1U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_crg->GetCrgErrCap()->GetCrgErrType()->GetDBG());
    ASSERT_EQ(err_um, m_crg->GetCrgErrCap()->GetCrgErrType()->GetUM());
    ASSERT_EQ(err_addr_bank1, m_crg->GetCrgErrCap()->GetCrgErrAddr()->GetAddr());
}


/*!
 * @brief Check CauseError method.
 * case 5:
 * Return SYSERR_CRG
 */

TEST_F(Crg5, Check_CauseError_PEnCLm) {
    uint32_t err_dm = 0x1, err_um = 0x1, err_spid = 0xa, err_spid_2 = 0x3;
    uint32_t err_addr_bank0 = 0xFFFFFFF0;
    SideBand sb_info;
    sb_info.SetBMID(0x0);
    sb_info.SetSPID(err_spid);
    sb_info.SetDM(err_dm);
    sb_info.SetUM(err_um);

    ASSERT_EQ(SYSERR_CRG, m_crg->CauseError(sb_info, err_addr_bank0,
                           DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_NE(nullptr, m_crg->GetCrgErrCap());

    // First error
    ASSERT_EQ(0xFFC6D800, m_crg->GetCrgErrCap()->m_reg_addr_base);

    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSEC());
    ASSERT_EQ(0x400U, m_crg->GetCrgErrCap()->GetCrgSpidErrStat()->GetSPIDERR());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_crg->GetCrgErrCap()->GetCrgErrType()->GetDBG());
    ASSERT_EQ(err_um, m_crg->GetCrgErrCap()->GetCrgErrType()->GetUM());
    ASSERT_EQ(err_addr_bank0, m_crg->GetCrgErrCap()->GetCrgErrAddr()->GetAddr());

    sb_info.SetSPID(err_spid_2);

    ASSERT_EQ(SYSERR_CRG, m_crg->CauseError(sb_info, err_addr_bank0,
                           DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_NE(nullptr, m_crg->GetCrgErrCap());
    ASSERT_EQ(0xFFC6D800, m_crg->GetCrgErrCap()->m_reg_addr_base);

    // Second error: Update OVF
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSEC());
    ASSERT_EQ(0x408U, m_crg->GetCrgErrCap()->GetCrgSpidErrStat()->GetSPIDERR());
    ASSERT_EQ(0x1U, m_crg->GetCrgErrCap()->GetCrgErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_crg->GetCrgErrCap()->GetCrgErrType()->GetDBG());
    ASSERT_EQ(err_um, m_crg->GetCrgErrCap()->GetCrgErrType()->GetUM());
    ASSERT_EQ(err_addr_bank0, m_crg->GetCrgErrCap()->GetCrgErrAddr()->GetAddr());
}


/*!
 * @brief Check CauseError method.
 * case 6:
 * Return SYSERR_CRG
 */

TEST_F(Crg5, Check_CauseError_SX2MBLCL_DiffPE) {
    uint32_t err_dm = 0x1, err_um = 0x1, err_spid = 0xa, err_spid_2 = 0x0;
    uint32_t err_addr_bank0 = 0xFFFFFFF0;
    SideBand sb_info;
    sb_info.SetBMID(0x1);
    sb_info.SetSPID(err_spid);
    sb_info.SetDM(err_dm);
    sb_info.SetUM(err_um);

    ASSERT_EQ(SYSERR_CRG, m_crg->CauseError(sb_info, err_addr_bank0,
                           DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_NE(nullptr, m_crg->GetCrgErrCap());

    // First error
    ASSERT_EQ(0xFFC6D930, m_crg->GetCrgErrCap()->m_reg_addr_base);

    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSEC());
    ASSERT_EQ(0x400U, m_crg->GetCrgErrCap()->GetCrgSpidErrStat()->GetSPIDERR());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_crg->GetCrgErrCap()->GetCrgErrType()->GetDBG());
    ASSERT_EQ(err_um, m_crg->GetCrgErrCap()->GetCrgErrType()->GetUM());
    ASSERT_EQ(err_addr_bank0, m_crg->GetCrgErrCap()->GetCrgErrAddr()->GetAddr());

    sb_info.SetSPID(err_spid_2);

    ASSERT_EQ(SYSERR_CRG, m_crg->CauseError(sb_info, err_addr_bank0,
                           DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_NE(nullptr, m_crg->GetCrgErrCap());
    ASSERT_EQ(0xFFC6D930, m_crg->GetCrgErrCap()->m_reg_addr_base);

    // Second error: Update OVF
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSEC());
    ASSERT_EQ(0x401U, m_crg->GetCrgErrCap()->GetCrgSpidErrStat()->GetSPIDERR());
    ASSERT_EQ(0x1U, m_crg->GetCrgErrCap()->GetCrgErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_crg->GetCrgErrCap()->GetCrgErrType()->GetDBG());
    ASSERT_EQ(err_um, m_crg->GetCrgErrCap()->GetCrgErrType()->GetUM());
    ASSERT_EQ(err_addr_bank0, m_crg->GetCrgErrCap()->GetCrgErrAddr()->GetAddr());
}


/*!
 * @brief Check CauseError method.
 * case 7:
 * Return SYSERR_CRG
 */

TEST_F(Crg5, Check_CauseError_SX2MBHCL_DiffPE) {
    uint32_t err_dm = 0x1, err_um = 0x1, err_spid = 0xa, err_spid_2 = 0x0;
    uint32_t err_addr_bank1 = 0xcafecafe;
    SideBand sb_info;
    sb_info.SetBMID(0x1);
    sb_info.SetSPID(err_spid);
    sb_info.SetDM(err_dm);
    sb_info.SetUM(err_um);

    ASSERT_EQ(SYSERR_CRG, m_crg->CauseError(sb_info, err_addr_bank1,
                           DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_NE(nullptr, m_crg->GetCrgErrCap());

    // First error
    ASSERT_EQ(0xFFC6D920, m_crg->GetCrgErrCap()->m_reg_addr_base);

    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSEC());
    ASSERT_EQ(0x400U, m_crg->GetCrgErrCap()->GetCrgSpidErrStat()->GetSPIDERR());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_crg->GetCrgErrCap()->GetCrgErrType()->GetDBG());
    ASSERT_EQ(err_um, m_crg->GetCrgErrCap()->GetCrgErrType()->GetUM());
    ASSERT_EQ(err_addr_bank1, m_crg->GetCrgErrCap()->GetCrgErrAddr()->GetAddr());

    sb_info.SetSPID(err_spid_2);

    ASSERT_EQ(SYSERR_CRG, m_crg->CauseError(sb_info, err_addr_bank1,
                           DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_NE(nullptr, m_crg->GetCrgErrCap());
    ASSERT_EQ(0xFFC6D920, m_crg->GetCrgErrCap()->m_reg_addr_base);

    // Second error: Update OVF
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSEC());
    ASSERT_EQ(0x401U, m_crg->GetCrgErrCap()->GetCrgSpidErrStat()->GetSPIDERR());
    ASSERT_EQ(0x1U, m_crg->GetCrgErrCap()->GetCrgErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_crg->GetCrgErrCap()->GetCrgErrType()->GetDBG());
    ASSERT_EQ(err_um, m_crg->GetCrgErrCap()->GetCrgErrType()->GetUM());
    ASSERT_EQ(err_addr_bank1, m_crg->GetCrgErrCap()->GetCrgErrAddr()->GetAddr());
}


/*!
 * @brief Check CauseError method.
 * case 8: Check error generated by same SPID with previous one
 * Return SYSERR_NONE
 */

TEST_F(Crg5, Check_CauseError_SX2MBHCL_DiffPE_sameSpid) {
    uint32_t err_dm = 0x1, err_um = 0x1, err_spid = 0x0, err_spid_2 = 0x0;
    uint32_t err_addr_bank1 = 0xcafecafe;
    SideBand sb_info;
    sb_info.SetBMID(0x1);
    sb_info.SetSPID(err_spid);
    sb_info.SetDM(err_dm);
    sb_info.SetUM(err_um);

    ASSERT_EQ(SYSERR_CRG, m_crg->CauseError(sb_info, err_addr_bank1,
                           DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_NE(nullptr, m_crg->GetCrgErrCap());

    // First error
    ASSERT_EQ(0xFFC6D920, m_crg->GetCrgErrCap()->m_reg_addr_base);

    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSEC());
    ASSERT_EQ(0x001U, m_crg->GetCrgErrCap()->GetCrgSpidErrStat()->GetSPIDERR());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_crg->GetCrgErrCap()->GetCrgErrType()->GetDBG());
    ASSERT_EQ(err_um, m_crg->GetCrgErrCap()->GetCrgErrType()->GetUM());
    ASSERT_EQ(err_addr_bank1, m_crg->GetCrgErrCap()->GetCrgErrAddr()->GetAddr());

    sb_info.SetSPID(err_spid_2);

    ASSERT_EQ(SYSERR_NONE, m_crg->CauseError(sb_info, err_addr_bank1,
                           DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_NE(nullptr, m_crg->GetCrgErrCap());
    ASSERT_EQ(0xFFC6D920, m_crg->GetCrgErrCap()->m_reg_addr_base);

    // Second error: Update OVF
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSEC());
    ASSERT_EQ(0x001U, m_crg->GetCrgErrCap()->GetCrgSpidErrStat()->GetSPIDERR());
    ASSERT_EQ(0x1U, m_crg->GetCrgErrCap()->GetCrgErrStat()->GetOVF());
    ASSERT_EQ(0x0U, m_crg->GetCrgErrCap()->GetCrgErrType()->GetWrite());
    ASSERT_EQ(err_spid, m_crg->GetCrgErrCap()->GetCrgErrType()->GetSPID());
    ASSERT_EQ(err_dm, m_crg->GetCrgErrCap()->GetCrgErrType()->GetDBG());
    ASSERT_EQ(err_um, m_crg->GetCrgErrCap()->GetCrgErrType()->GetUM());
    ASSERT_EQ(err_addr_bank1, m_crg->GetCrgErrCap()->GetCrgErrAddr()->GetAddr());
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
