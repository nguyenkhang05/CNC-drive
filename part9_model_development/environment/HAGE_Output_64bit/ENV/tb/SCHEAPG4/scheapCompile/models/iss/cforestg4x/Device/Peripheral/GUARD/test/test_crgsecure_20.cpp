/**
 * @file test_crgsecure_20.cpp
 *
 * @date Created on: 2018
 * @author Author: Hoanh Ho <hoanh.ho.px@renesas.com>
 * @brief UNIT TEST for GKCERRADDR. (SAMPLE)
 * @par Copyright: 2018 Renesas Electronics Corp.
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

class CrgSecure;

static const uint32_t CL0_CRG_UT = 0x0;
static const uint32_t CL1_CRG_UT = 0x1;

// ----------------------------------------
class CrgSecure1 : public testing::Test {
protected:
    CrgSecure1() {}
    virtual ~CrgSecure1() {}
    virtual void SetUp() {
        GuardInternal * guard_internal= new GuardInternal(false, false,
                                                              0x13, 0x8, 0x8, 0x4, 0xA5A5A500, 0x0);
        m_crgsecure           = new CrgSecure (base_crgreg_csg, CORE_G4MH, CORE_VER_20, guard_internal);
    }
    virtual void TearDown() { delete m_crgsecure; }

public:
    CrgSecure          *m_crgsecure;
    MockSideBand sideband;

public:
    uint32_t m_peid = 0x0;
    uint32_t base_crgreg_csg = 0xFFC6D000;
    union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
    };
};


/*!
 * @brief Check Initial method.
 */

TEST_F(CrgSecure1, Check_Initial_Setup) {
    ASSERT_NE(nullptr, m_crgsecure->GetCrgSecureReg());
    ASSERT_FALSE(m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->IsGuardEn());
}


/*!
 * @brief Check Reset method.
 */
TEST_F(CrgSecure1, Check_Reset) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    m_crgsecure->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET, new uint8_t[4] {0x00, 0x00, 0x00, 0x80}, SIZE_32BIT);
    ASSERT_EQ(0x1U, m_crgsecure->GetCrgSecureReg()->GetCrgProt(chn_0)->GetLOCK());
    m_crgsecure->HardReset();
    ASSERT_EQ(0x0U, m_crgsecure->GetCrgSecureReg()->GetCrgProt(chn_0)->GetLOCK());
}


/*!
 * @brief Check TargetWrite method.
 */
TEST_F(CrgSecure1, Check_TargetWrite) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;

    m_crgsecure->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x53, 0x01, 0x00, 0x00}, SIZE_32BIT);
    // at least, 1 CRG channel setting is enable.

    m_crgsecure->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crgsecure->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crgsecure->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);

    ASSERT_EQ(0x1U, m_crgsecure->GetCrgSecureReg()->GetCrgProt(chn_0)->GetGEN());
    ASSERT_EQ(0x0U, m_crgsecure->GetCrgSecureReg()->GetCrgProt(chn_0)->GetLOCK());
    ASSERT_EQ(0xFFFFFFFFU, m_crgsecure->GetCrgSecureReg()->GetCrgSpid(chn_0)->GetSPID());
    ASSERT_EQ(0x0007FF00U, m_crgsecure->GetCrgSecureReg()->GetCrgBad(chn_0)->GetBAD());
    ASSERT_EQ(0x0007FF00U, m_crgsecure->GetCrgSecureReg()->GetCrgAdv(chn_0)->GetADV());

    // Check guard internal value.
    m_crgsecure->GetCrgSecureReg()->GetCrgProt(0)->SetGEN(1);
    m_crgsecure->GetCrgSecureReg()->GetCrgProt(1)->SetGEN(1);
    m_crgsecure->GetCrgSecureReg()->GetCrgProt(2)->SetGEN(1);
    m_crgsecure->GetCrgSecureReg()->GetCrgProt(3)->SetGEN(1);
    ASSERT_TRUE(m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->IsChannelEn(0));
    ASSERT_TRUE(m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->IsChannelEn(1));
    ASSERT_TRUE(m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->IsChannelEn(2));
    ASSERT_TRUE(m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->IsChannelEn(3));
    ASSERT_FALSE(m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->IsGuardEn());
}


/*!
 * @brief Check TargetRead method.
 */
TEST_F(CrgSecure1, Check_TargetRead) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    uint8_t data_8_input_crgprot[4] = {0x53, 0x01, 0x00, 0x00};
    uint8_t data_8_input_crgspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_crgbad[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_crgadv[4] = {0x00, 0xff, 0x07, 0x00};

    uint8_t data_8_output_crgprot[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgbad[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgadv[4]    = {0xff, 0xff, 0xff, 0xff};

    m_crgsecure->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_crgprot, SIZE_32BIT);
    m_crgsecure->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_crgspid, SIZE_32BIT);
    m_crgsecure->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_crgbad, SIZE_32BIT);
    m_crgsecure->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_crgadv, SIZE_32BIT);

    m_crgsecure->TargetRead(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0, data_8_output_crgprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgprot, sizeof(m_data32));
    uint32_t m_data32_out_crgprot = m_data32;
    memcpy (&m_data32, data_8_input_crgprot, sizeof(m_data32));
    uint32_t m_data32_in_crgprot = m_data32;
    ASSERT_EQ(m_data32_out_crgprot, m_data32_in_crgprot);

    m_crgsecure->TargetRead(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0, data_8_output_crgspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgspid, sizeof(m_data32));
    uint32_t m_data32_out_crgspid = m_data32;
    memcpy (&m_data32, data_8_input_crgspid, sizeof(m_data32));
    uint32_t m_data32_in_crgspid = m_data32;
    ASSERT_EQ(m_data32_out_crgspid, m_data32_in_crgspid);

    m_crgsecure->TargetRead(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0, data_8_output_crgbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgbad, sizeof(m_data32));
    uint32_t m_data32_out_crgbad = m_data32;
    memcpy (&m_data32, data_8_input_crgbad, sizeof(m_data32));
    uint32_t m_data32_in_crgbad = m_data32;
    ASSERT_EQ(m_data32_out_crgbad, m_data32_in_crgbad);

    m_crgsecure->TargetRead(sb_info,  base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0, data_8_output_crgadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgadv, sizeof(m_data32));
    uint32_t m_data32_out_crgadv = m_data32;
    memcpy (&m_data32, data_8_input_crgadv, sizeof(m_data32));
    uint32_t m_data32_in_crgadv = m_data32;
    ASSERT_EQ(m_data32_out_crgadv, m_data32_in_crgadv);
}


/*!
 * @brief Check TargetFetch method.
 */
TEST_F(CrgSecure1, Check_TargetFetch) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    uint8_t data_8_input_crgprot[4] = {0x53, 0x01, 0x00, 0x00};
    uint8_t data_8_input_crgspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_crgbad[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_crgadv[4] = {0x00, 0xff, 0x07, 0x00};

    uint8_t data_8_output_crgprot[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgbad[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgadv[4]    = {0xff, 0xff, 0xff, 0xff};

    m_crgsecure->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_crgprot, SIZE_32BIT);
    m_crgsecure->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_crgspid, SIZE_32BIT);
    m_crgsecure->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_crgbad, SIZE_32BIT);
    m_crgsecure->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_crgadv, SIZE_32BIT);

    m_crgsecure->TargetFetch(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0, data_8_output_crgprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgprot, sizeof(m_data32));
    uint32_t m_data32_out_crgprot = m_data32;
    memcpy (&m_data32, data_8_input_crgprot, sizeof(m_data32));
    uint32_t m_data32_in_crgprot = m_data32;
    ASSERT_EQ(m_data32_out_crgprot, m_data32_in_crgprot);

    m_crgsecure->TargetFetch(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0, data_8_output_crgspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgspid, sizeof(m_data32));
    uint32_t m_data32_out_crgspid = m_data32;
    memcpy (&m_data32, data_8_input_crgspid, sizeof(m_data32));
    uint32_t m_data32_in_crgspid = m_data32;
    ASSERT_EQ(m_data32_out_crgspid, m_data32_in_crgspid);

    m_crgsecure->TargetFetch(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0, data_8_output_crgbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgbad, sizeof(m_data32));
    uint32_t m_data32_out_crgbad = m_data32;
    memcpy (&m_data32, data_8_input_crgbad, sizeof(m_data32));
    uint32_t m_data32_in_crgbad = m_data32;
    ASSERT_EQ(m_data32_out_crgbad, m_data32_in_crgbad);

    m_crgsecure->TargetFetch(sb_info,  base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0, data_8_output_crgadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgadv, sizeof(m_data32));
    uint32_t m_data32_out_crgadv = m_data32;
    memcpy (&m_data32, data_8_input_crgadv, sizeof(m_data32));
    uint32_t m_data32_in_crgadv = m_data32;
    ASSERT_EQ(m_data32_out_crgadv, m_data32_in_crgadv);
}


/*!
 * @brief Check TargetWriteDebug method.
 */
TEST_F(CrgSecure1, Check_TargetWriteDebug) {
    uint32_t chn_0 = 0x1;

    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x53, 0x01, 0x00, 0x00}, SIZE_32BIT);
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);

    ASSERT_EQ(0x1U, m_crgsecure->GetCrgSecureReg()->GetCrgProt(chn_0)->GetGEN());
    ASSERT_EQ(0xFFFFFFFFU, m_crgsecure->GetCrgSecureReg()->GetCrgSpid(chn_0)->GetSPID());
    ASSERT_EQ(0x0007FF00U, m_crgsecure->GetCrgSecureReg()->GetCrgBad(chn_0)->GetBAD());
    ASSERT_EQ(0x0007FF00U, m_crgsecure->GetCrgSecureReg()->GetCrgAdv(chn_0)->GetADV());

    // Check guard internal value.
    m_crgsecure->GetCrgSecureReg()->GetCrgProt(0)->SetGEN(1);
    m_crgsecure->GetCrgSecureReg()->GetCrgProt(1)->SetGEN(1);
    m_crgsecure->GetCrgSecureReg()->GetCrgProt(2)->SetGEN(1);
    m_crgsecure->GetCrgSecureReg()->GetCrgProt(3)->SetGEN(1);
    ASSERT_TRUE(m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->IsChannelEn(0));
    ASSERT_TRUE(m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->IsChannelEn(1));
    ASSERT_TRUE(m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->IsChannelEn(2));
    ASSERT_TRUE(m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->IsChannelEn(3));
    ASSERT_FALSE(m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->IsGuardEn());
}


/*!
 * @brief Check TargetReadDebug method.
 */
TEST_F(CrgSecure1, Check_TargetReadDebug) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    uint8_t data_8_input_crgprot[4] = {0x53, 0x01, 0x00, 0x00};
    uint8_t data_8_input_crgspid[4] = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_input_crgbad[4] = {0x00, 0xff, 0x07, 0x00};
    uint8_t data_8_input_crgadv[4] = {0x00, 0xff, 0x07, 0x00};

    uint8_t data_8_output_crgprot[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgspid[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgbad[4]    = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgadv[4]    = {0xff, 0xff, 0xff, 0xff};

    m_crgsecure->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_crgprot, SIZE_32BIT);
    m_crgsecure->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_crgspid, SIZE_32BIT);
    m_crgsecure->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_crgbad, SIZE_32BIT);
    m_crgsecure->TargetWrite(sb_info, base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                           data_8_input_crgadv, SIZE_32BIT);

    m_crgsecure->TargetReadDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0, data_8_output_crgprot, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgprot, sizeof(m_data32));
    uint32_t m_data32_out_crgprot = m_data32;
    memcpy (&m_data32, data_8_input_crgprot, sizeof(m_data32));
    uint32_t m_data32_in_crgprot = m_data32;
    ASSERT_EQ(m_data32_out_crgprot, m_data32_in_crgprot);

    m_crgsecure->TargetReadDebug(base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0, data_8_output_crgspid, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgspid, sizeof(m_data32));
    uint32_t m_data32_out_crgspid = m_data32;
    memcpy (&m_data32, data_8_input_crgspid, sizeof(m_data32));
    uint32_t m_data32_in_crgspid = m_data32;
    ASSERT_EQ(m_data32_out_crgspid, m_data32_in_crgspid);

    m_crgsecure->TargetReadDebug(base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0, data_8_output_crgbad, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgbad, sizeof(m_data32));
    uint32_t m_data32_out_crgbad = m_data32;
    memcpy (&m_data32, data_8_input_crgbad, sizeof(m_data32));
    uint32_t m_data32_in_crgbad = m_data32;
    ASSERT_EQ(m_data32_out_crgbad, m_data32_in_crgbad);

    m_crgsecure->TargetReadDebug(base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0, data_8_output_crgadv, SIZE_32BIT);
    memcpy (&m_data32, data_8_output_crgadv, sizeof(m_data32));
    uint32_t m_data32_out_crgadv = m_data32;
    memcpy (&m_data32, data_8_input_crgadv, sizeof(m_data32));
    uint32_t m_data32_in_crgadv = m_data32;
    ASSERT_EQ(m_data32_out_crgadv, m_data32_in_crgadv);
}


// ----------------------------------------
class CrgSecure2 : public testing::Test {
protected:
    CrgSecure2() {}
    virtual ~CrgSecure2() {}
    virtual void SetUp() {
        GuardInternal * guard_internal= new GuardInternal(false, false,
                                                              0x13, 0x8, 0x8, 0x4, 0xA5A5A500, 0x0);
        m_crgsecure           = new CrgSecure (base_crgreg_csg, CORE_G4MH, CORE_VER_20, guard_internal);
    }
    virtual void TearDown() { delete m_crgsecure; }

public:
    CrgSecure          *m_crgsecure;
    MockSideBand sideband;

public:
    uint32_t m_clid = CL0_CRG_UT;
    const uint32_t base_crgreg_csg = 0xFFC6E000;
};


/*!
 * @brief Check IsAddressMatch method.
 * This function is only used to check in area [0xFC000000, 0xFFFFFFFF].
 */

TEST_F(CrgSecure2, Check_IsAddressMatch_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Set BAD = 0xFC00_0000, ADV = 0x0000_0000.
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    ASSERT_TRUE(m_crgsecure->IsAddressMatch(chn_0, 0xFC000000, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crgsecure->IsAddressMatch(chn_0, 0xFC000004, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crgsecure->IsAddressMatch(chn_0, 0xFC000FFF, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crgsecure->IsAddressMatch(chn_0, 0xFC00FFFF, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crgsecure->IsAddressMatch(chn_0, 0xFC0FFFFF, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crgsecure->IsAddressMatch(chn_0, 0xFCFFFFFF, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crgsecure->IsAddressMatch(chn_0, 0xFFFFFFFF, CORE_G4MH, CORE_VER_20));

    // Set ADV = 0x03FFF000,
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);
    ASSERT_TRUE(m_crgsecure->IsAddressMatch(chn_0, 0xFC000000, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crgsecure->IsAddressMatch(chn_0, 0xFC000004, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crgsecure->IsAddressMatch(chn_0, 0xFC000FFF, CORE_G4MH, CORE_VER_20));
}


TEST_F(CrgSecure2, Check_IsAddressMatch_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Set BAD = 0xFC00_0000, ADV = 0x0000_0000.
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);

    // Set ADV = 0x03FFF000,
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);

    ASSERT_FALSE(m_crgsecure->IsAddressMatch(chn_0, 0xFC00FFFF, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crgsecure->IsAddressMatch(chn_0, 0xFC0FFFFF, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crgsecure->IsAddressMatch(chn_0, 0xFCFFFFFF, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crgsecure->IsAddressMatch(chn_0, 0xFFFFFFFF, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crgsecure->IsAddressMatch(chn_0, 0xFFFFFFFF, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsSpidAllowed method.
 */

TEST_F(CrgSecure2, Check_IsSpidAllowed_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable SPID
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetSPID(0);
    ASSERT_FALSE(m_crgsecure->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
    sb_info.SetSPID(31);
    ASSERT_FALSE(m_crgsecure->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
    sb_info.SetSPID(15);
    ASSERT_FALSE(m_crgsecure->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));

}

TEST_F(CrgSecure2, Check_IsSpidAllowed_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable SPID
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x01, 0x80, 0x00, 0x80}, SIZE_32BIT);
    sb_info.SetSPID(0);
    ASSERT_TRUE(m_crgsecure->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
    sb_info.SetSPID(31);
    ASSERT_TRUE(m_crgsecure->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
    sb_info.SetSPID(15);
    ASSERT_TRUE(m_crgsecure->IsSpidAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));

}


/*!
 * @brief Check IsWriteGloAllowed method.
 */

TEST_F(CrgSecure2, Check_IsWriteGloAllowed_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable WG
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);

    ASSERT_FALSE(m_crgsecure->IsWriteGloAllowed(chn_0, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crgsecure->IsWriteGloAllowed(chn_0, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crgsecure->IsWriteGloAllowed(chn_0, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crgsecure->IsWriteGloAllowed(chn_0, DATA_RMW, CORE_G4MH, CORE_VER_20));
}

TEST_F(CrgSecure2, Check_IsWriteGloAllowed_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable WG
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x02, 0x00, 0x00, 0x00}, SIZE_32BIT);

    ASSERT_FALSE(m_crgsecure->IsWriteGloAllowed(chn_0, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crgsecure->IsWriteGloAllowed(chn_0, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crgsecure->IsWriteGloAllowed(chn_0, DATA_RMW, CORE_G4MH, CORE_VER_20));
}

TEST_F(CrgSecure2, Check_IsWriteGloAllowed_3) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable WG
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x02, 0x00, 0x00, 0x00}, SIZE_32BIT);

    ASSERT_TRUE(m_crgsecure->IsWriteGloAllowed(chn_0, DATA_WRITE, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsReadGloAllowed method.
 */

TEST_F(CrgSecure2, Check_IsReadGloAllowed_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable RG
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);

    ASSERT_FALSE(m_crgsecure->IsReadGloAllowed(chn_0, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crgsecure->IsReadGloAllowed(chn_0, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crgsecure->IsReadGloAllowed(chn_0, INST_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crgsecure->IsReadGloAllowed(chn_0, DATA_RMW, CORE_G4MH, CORE_VER_20));
}

TEST_F(CrgSecure2, Check_IsReadGloAllowed_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable RG
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x01, 0x00, 0x00, 0x00}, SIZE_32BIT);

    ASSERT_FALSE(m_crgsecure->IsReadGloAllowed(chn_0, DATA_WRITE, CORE_G4MH, CORE_VER_20));
    ASSERT_FALSE(m_crgsecure->IsReadGloAllowed(chn_0, DATA_RMW, CORE_G4MH, CORE_VER_20));
}

TEST_F(CrgSecure2, Check_IsReadGloAllowed_3) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable RG
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x01, 0x00, 0x00, 0x00}, SIZE_32BIT);

    ASSERT_TRUE(m_crgsecure->IsReadGloAllowed(chn_0, DATA_READ, CORE_G4MH, CORE_VER_20));
    ASSERT_TRUE(m_crgsecure->IsReadGloAllowed(chn_0, INST_READ, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsUMDisabled method.
 */

TEST_F(CrgSecure2, Check_IsUMDisabled_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable UME
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetUM(0);
    ASSERT_FALSE(m_crgsecure->IsUMDisabled(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
    sb_info.SetUM(1);
    ASSERT_TRUE(m_crgsecure->IsUMDisabled(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
}

TEST_F(CrgSecure2, Check_IsUMDisabled_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable UME
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x10, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetUM(0);
    ASSERT_FALSE(m_crgsecure->IsUMDisabled(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
    sb_info.SetUM(1);
    ASSERT_FALSE(m_crgsecure->IsUMDisabled(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
}


/*!
 * @brief Check IsDbgAllowed method.
 */

TEST_F(CrgSecure2, Check_IsDbgAllowed_1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable DB permission
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetDM(0);
    ASSERT_FALSE(m_crgsecure->IsDbgAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
    sb_info.SetDM(1);
    ASSERT_FALSE(m_crgsecure->IsDbgAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
}

TEST_F(CrgSecure2, Check_IsDbgAllowed_2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    // Disable DB permission
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x40, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetDM(0);
    ASSERT_FALSE(m_crgsecure->IsDbgAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
    sb_info.SetDM(1);
    ASSERT_TRUE(m_crgsecure->IsDbgAllowed(chn_0, sb_info, CORE_G4MH, CORE_VER_20));
}


// ----------------------------------------
// CrgSecure3 is used to checked IsMemAccPermit in case PE of memory access is PE contains CSG.

class CrgSecure3 : public testing::Test {
protected:
    CrgSecure3() {}
    virtual ~CrgSecure3() {}
    virtual void SetUp() {
        GuardInternal * guard_internal= new GuardInternal(false, false,
                                                              0x13, 0x8, 0x8, 0x4, 0xA5A5A500, 0x0);
        m_crgsecure           = new CrgSecure (base_crgreg_csg, CORE_G4MH, CORE_VER_20, guard_internal);
    }
    virtual void TearDown() { delete m_crgsecure; }

public:
    CrgSecure          *m_crgsecure;
    MockSideBand sideband;

public:
    uint32_t m_clid = CL0_CRG_UT;
    const uint32_t base_crgreg_csg = 0xFFC6E000;
};


/*!
 * @brief Check IsMemAccPermit method.
 * case 3.1: Address at channel 0 is match & DBG is allowed.
 * Return SYSERR_NONE
 */

TEST_F(CrgSecure3, Check_IsMemAccPermit_AddrMatch_ch0) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x40, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetDM(1);
    m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->Reset();
    m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->SetChannelEn(chn_0, true);
    sb_info.SetBitOp(SideBand::BITOP_NONE);
    ASSERT_EQ(SYSERR_NONE, m_crgsecure->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20, chn_0));
    ASSERT_EQ(SYSERR_NONE, m_crgsecure->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20, chn_0));
    ASSERT_EQ(SYSERR_NONE, m_crgsecure->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20, chn_0));
    ASSERT_EQ(SYSERR_NONE, m_crgsecure->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_RMW, CORE_G4MH, CORE_VER_20, chn_0));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 3.2: Address at channel 0 is match & UME is disabled.
 * Return SYSERR_NONE
 */

TEST_F(CrgSecure3, Check_IsMemAccPermit_AddrMatch_ch1) {
    SideBand sb_info;
    uint32_t chn_0 = 0x1;
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetUM(1);
    m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->Reset();
    m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->SetChannelEn(chn_0, true);
    sb_info.SetBitOp(SideBand::BITOP_NONE);
    ASSERT_EQ(SYSERR_NONE, m_crgsecure->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20, chn_0));
    ASSERT_EQ(SYSERR_NONE, m_crgsecure->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20, chn_0));
    ASSERT_EQ(SYSERR_NONE, m_crgsecure->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20, chn_0));
    ASSERT_EQ(SYSERR_NONE, m_crgsecure->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_RMW, CORE_G4MH, CORE_VER_20, chn_0));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 3.3: Address at channel 0 is match & SPID is allowed.
 * Return SYSERR_NONE
 */

TEST_F(CrgSecure3, Check_IsMemAccPermit_AddrMatch_ch2) {
    SideBand sb_info;
    uint32_t chn_0 = 0x2;
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x01, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetSPID(0);
    m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->Reset();
    m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->SetChannelEn(chn_0, true);
    sb_info.SetBitOp(SideBand::BITOP_NONE);
    ASSERT_EQ(SYSERR_NONE, m_crgsecure->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20, chn_0));
    ASSERT_EQ(SYSERR_NONE, m_crgsecure->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20, chn_0));
    ASSERT_EQ(SYSERR_NONE, m_crgsecure->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20, chn_0));
    ASSERT_EQ(SYSERR_NONE, m_crgsecure->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_RMW, CORE_G4MH, CORE_VER_20, chn_0));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 3.4: Address at channel 0 is match & RG||WG is allowed.
 * Return SYSERR_NONE
 */

TEST_F(CrgSecure3, Check_IsMemAccPermit_AddrMatch_ch3) {
    SideBand sb_info;
    uint32_t chn_0 = 0x3;
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x03, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetSPID(10);
    m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->Reset();
    m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->SetChannelEn(chn_0, true);
    sb_info.SetBitOp(SideBand::BITOP_NONE);
    ASSERT_EQ(SYSERR_NONE, m_crgsecure->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20, chn_0));
    ASSERT_EQ(SYSERR_NONE, m_crgsecure->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20, chn_0));
    ASSERT_EQ(SYSERR_NONE, m_crgsecure->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20, chn_0));
}


/*!
 * @brief Check IsMemAccPermit method.
 * case 3.5: Address at channel 0 is match but all are not allowed.
 * Return SYSERR_NONE
 */

TEST_F(CrgSecure3, Check_IsMemAccPermit_AddrMatch_ch4) {
    SideBand sb_info;
    uint32_t chn_0 = 0x0;
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGBAD_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0x00, 0x00, 0xFC}, SIZE_32BIT);
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGADV_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x00, 0xF0, 0xFF, 0x03}, SIZE_32BIT);
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGPROT_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x10, 0x00, 0x00, 0x00}, SIZE_32BIT);
    m_crgsecure->TargetWriteDebug(base_crgreg_csg + TRegCrgSecureReg::CSGSPID_OFFSET + TRegCrgSecureReg::CRG_SEC_CHANNEL_RANGE * chn_0,
                                    new uint8_t[4] {0x10, 0x00, 0x00, 0x00}, SIZE_32BIT);
    sb_info.SetSPID(3);
    sb_info.SetUM(1);
    m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->Reset();
    m_crgsecure->GetCrgSecureReg()->GetCrgInternal()->SetChannelEn(chn_0, true);
    sb_info.SetBitOp(SideBand::BITOP_NONE);
    ASSERT_EQ(SYSERR_CRG, m_crgsecure->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, DATA_WRITE, CORE_G4MH, CORE_VER_20, chn_0));
    ASSERT_EQ(SYSERR_CRG, m_crgsecure->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_READ, CORE_G4MH, CORE_VER_20, chn_0));
    ASSERT_EQ(SYSERR_CRG, m_crgsecure->IsMemAccPermit(sb_info, 0xFC0FFFFF,
              SIZE_32BIT, INST_READ, CORE_G4MH, CORE_VER_20, chn_0));
    ASSERT_EQ(SYSERR_CRG, m_crgsecure->IsMemAccPermit(sb_info, 0xFCFFFFFF,
              SIZE_32BIT, DATA_RMW, CORE_G4MH, CORE_VER_20, chn_0));
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
