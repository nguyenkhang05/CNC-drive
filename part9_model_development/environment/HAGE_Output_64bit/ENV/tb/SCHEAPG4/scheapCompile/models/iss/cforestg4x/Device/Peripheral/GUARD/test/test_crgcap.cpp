/**
 * @file test_crgcap.cpp
 *
 * @date Created on: 2016
 * @author Author: Quang Lam <quang.lam.px@rvc.renesas.com>
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

class CrgCap;

// ----------------------------------------
class CrgCap1 : public testing::Test {
protected:
    CrgCap1() {}
    virtual ~CrgCap1() {}
    virtual void SetUp() {
        m_crgcap           = new CrgCap (base_crgcapreg, CORE_G4MH, CORE_VER_11);
    }
    virtual void TearDown() { delete m_crgcap; }

public:
    CrgCap          *m_crgcap;
    MockSideBand sideband;

public:
    uint32_t m_peid = 0x0;
    uint32_t base_crgcapreg = 0xFFC6D800;
    union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
    };
};


/*!
 * @brief Check Initial method.
 */

TEST_F(CrgCap1, Check_Initial_Setup) {
    ASSERT_NE(nullptr, m_crgcap->GetCrgErrReg());
}


/*!
 * @brief Check Reset method.
 */
TEST_F(CrgCap1, Check_Reset) {
    SideBand sb_info;
    // Change addr value in CRGADDR to confirm if it is reset or not.
    m_crgcap->GetCrgErrReg()->GetCrgErrAddr()->SetAddr(0xcafecafe);
    m_crgcap->HardReset();
    ASSERT_EQ(0x0U, m_crgcap->GetCrgErrReg()->GetCrgErrAddr()->GetAddr());
}


/*!
 * @brief Check TargetWrite method.
 */
TEST_F(CrgCap1, Check_TargetWrite) {
    SideBand sb_info;
    ASSERT_THROW(m_crgcap->TargetWrite(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRADDR_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_crgcap->TargetWrite(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRSTAT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_crgcap->TargetWrite(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRTYPE_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_crgcap->TargetWrite(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRCLR_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);

    ASSERT_EQ(SYSERR_NONE, m_crgcap->TargetWrite(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRADDR_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT));
    ASSERT_EQ(SYSERR_NONE, m_crgcap->TargetWrite(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRSTAT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT));
    ASSERT_EQ(SYSERR_NONE, m_crgcap->TargetWrite(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRTYPE_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT));
    ASSERT_EQ(SYSERR_NONE, m_crgcap->TargetWrite(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRCLR_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT));
}


/*!
 * @brief Check TargetRead method.
 */
TEST_F(CrgCap1, Check_TargetRead) {
    SideBand sb_info;

    uint8_t data_8_output_crgaddr[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgstat[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgtype[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgclr [4]   = {0xff, 0xff, 0xff, 0xff};

    m_crgcap->GetCrgErrReg()->GetCrgErrAddr()->SetAddr(0xcafecafe);
    m_crgcap->GetCrgErrReg()->GetCrgErrStat()->SetERR(0x1);
    m_crgcap->GetCrgErrReg()->GetCrgErrType()->SetWrite(0x1);


    m_crgcap->TargetRead(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRADDR_OFFSET, data_8_output_crgaddr, SIZE_32BIT);
    m_crgcap->TargetRead(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRSTAT_OFFSET, data_8_output_crgstat, SIZE_32BIT);
    m_crgcap->TargetRead(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRTYPE_OFFSET, data_8_output_crgtype, SIZE_32BIT);
    m_crgcap->TargetRead(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRCLR_OFFSET,  data_8_output_crgclr, SIZE_32BIT);

    memcpy (&m_data32, data_8_output_crgaddr, sizeof(m_data32));
    uint32_t m_data32_out_crgaddr = m_data32;
    memcpy (&m_data32, data_8_output_crgstat, sizeof(m_data32));
    uint32_t m_data32_out_crgstat = m_data32;
    memcpy (&m_data32, data_8_output_crgtype, sizeof(m_data32));
    uint32_t m_data32_out_crgtype = m_data32;
    memcpy (&m_data32, data_8_output_crgclr, sizeof(m_data32));
    uint32_t m_data32_out_crgclr = m_data32;

    ASSERT_EQ(0xcafecafe, m_data32_out_crgaddr);
    ASSERT_EQ(0x1U, m_data32_out_crgstat);
    ASSERT_EQ(0x1U, m_data32_out_crgtype);
    ASSERT_EQ(0x0U, m_data32_out_crgclr);
}


/*!
 * @brief Check TargetFetch method.
 */
TEST_F(CrgCap1, Check_TargetFetch) {
    SideBand sb_info;

    uint8_t data_8_output_crgaddr[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgstat[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgtype[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgclr [4]   = {0xff, 0xff, 0xff, 0xff};

    m_crgcap->GetCrgErrReg()->GetCrgErrAddr()->SetAddr(0xcafecafe);
    m_crgcap->GetCrgErrReg()->GetCrgErrStat()->SetERR(0x1);
    m_crgcap->GetCrgErrReg()->GetCrgErrType()->SetWrite(0x1);


    m_crgcap->TargetFetch(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRADDR_OFFSET, data_8_output_crgaddr, SIZE_32BIT);
    m_crgcap->TargetFetch(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRSTAT_OFFSET, data_8_output_crgstat, SIZE_32BIT);
    m_crgcap->TargetFetch(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRTYPE_OFFSET, data_8_output_crgtype, SIZE_32BIT);
    m_crgcap->TargetFetch(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRCLR_OFFSET,  data_8_output_crgclr, SIZE_32BIT);

    memcpy (&m_data32, data_8_output_crgaddr, sizeof(m_data32));
    uint32_t m_data32_out_crgaddr = m_data32;
    memcpy (&m_data32, data_8_output_crgstat, sizeof(m_data32));
    uint32_t m_data32_out_crgstat = m_data32;
    memcpy (&m_data32, data_8_output_crgtype, sizeof(m_data32));
    uint32_t m_data32_out_crgtype = m_data32;
    memcpy (&m_data32, data_8_output_crgclr, sizeof(m_data32));
    uint32_t m_data32_out_crgclr = m_data32;

    ASSERT_EQ(0xcafecafe, m_data32_out_crgaddr);
    ASSERT_EQ(0x1U, m_data32_out_crgstat);
    ASSERT_EQ(0x1U, m_data32_out_crgtype);
    ASSERT_EQ(0x0U, m_data32_out_crgclr);
}


/*!
 * @brief Check TargetWriteDebug method.
 */
TEST_F(CrgCap1, Check_TargetWriteDebug) {
    SideBand sb_info;
    uint8_t data_8_output_crgaddr[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgstat[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgtype[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgclr [4]   = {0xff, 0xff, 0xff, 0xff};

    m_crgcap->TargetWriteDebug(base_crgcapreg + TRegCrgErrReg::CRGERRADDR_OFFSET, new uint8_t[4] {0xfe, 0xca, 0xfe, 0xca}, SIZE_32BIT);
    m_crgcap->TargetWriteDebug(base_crgcapreg + TRegCrgErrReg::CRGERRSTAT_OFFSET, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crgcap->TargetWriteDebug(base_crgcapreg + TRegCrgErrReg::CRGERRTYPE_OFFSET, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crgcap->TargetWriteDebug(base_crgcapreg + TRegCrgErrReg::CRGERRCLR_OFFSET,  new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);

    m_crgcap->TargetRead(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRADDR_OFFSET, data_8_output_crgaddr, SIZE_32BIT);
    m_crgcap->TargetRead(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRSTAT_OFFSET, data_8_output_crgstat, SIZE_32BIT);
    m_crgcap->TargetRead(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRTYPE_OFFSET, data_8_output_crgtype, SIZE_32BIT);
    m_crgcap->TargetRead(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRCLR_OFFSET,  data_8_output_crgclr, SIZE_32BIT);

    memcpy (&m_data32, data_8_output_crgaddr, sizeof(m_data32));
    uint32_t m_data32_out_crgaddr = m_data32;
    memcpy (&m_data32, data_8_output_crgstat, sizeof(m_data32));
    uint32_t m_data32_out_crgstat = m_data32;
    memcpy (&m_data32, data_8_output_crgtype, sizeof(m_data32));
    uint32_t m_data32_out_crgtype = m_data32;
    memcpy (&m_data32, data_8_output_crgclr, sizeof(m_data32));
    uint32_t m_data32_out_crgclr = m_data32;

    // Defined WriteBody function
    ASSERT_EQ(0x0U, m_data32_out_crgclr);

    // Didn't define WriteBody function, so, the read value is always 0.
    ASSERT_EQ(0x0U, m_data32_out_crgaddr);
    ASSERT_EQ(0x0U, m_data32_out_crgtype);
    ASSERT_EQ(0x0U, m_data32_out_crgstat);

    m_crgcap->TargetWriteDebug(base_crgcapreg + TRegCrgErrReg::CRGERRADDR_OFFSET, new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    m_crgcap->TargetWriteDebug(base_crgcapreg + TRegCrgErrReg::CRGERRSTAT_OFFSET, new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    m_crgcap->TargetWriteDebug(base_crgcapreg + TRegCrgErrReg::CRGERRTYPE_OFFSET, new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    m_crgcap->TargetWriteDebug(base_crgcapreg + TRegCrgErrReg::CRGERRCLR_OFFSET,  new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);

    m_crgcap->TargetRead(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRADDR_OFFSET, data_8_output_crgaddr, SIZE_32BIT);
    m_crgcap->TargetRead(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRSTAT_OFFSET, data_8_output_crgstat, SIZE_32BIT);
    m_crgcap->TargetRead(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRTYPE_OFFSET, data_8_output_crgtype, SIZE_32BIT);
    m_crgcap->TargetRead(sb_info, base_crgcapreg + TRegCrgErrReg::CRGERRCLR_OFFSET,  data_8_output_crgclr, SIZE_32BIT);

    memcpy (&m_data32, data_8_output_crgaddr, sizeof(m_data32));
    m_data32_out_crgaddr = m_data32;
    memcpy (&m_data32, data_8_output_crgstat, sizeof(m_data32));
    m_data32_out_crgstat = m_data32;
    memcpy (&m_data32, data_8_output_crgtype, sizeof(m_data32));
    m_data32_out_crgtype = m_data32;
    memcpy (&m_data32, data_8_output_crgclr, sizeof(m_data32));
    m_data32_out_crgclr = m_data32;

    // Defined WriteBody function
    ASSERT_EQ(0x0U, m_data32_out_crgclr);
    ASSERT_EQ(0x0U, m_data32_out_crgaddr);
    ASSERT_EQ(0x0U, m_data32_out_crgtype);
    ASSERT_EQ(0x0U, m_data32_out_crgstat);
}


/*!
 * @brief Check TargetReadDebug method.
 */
TEST_F(CrgCap1, Check_TargetReadDebug) {
    SideBand sb_info;
    uint8_t data_8_output_crgaddr[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgstat[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgtype[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_crgclr [4]   = {0xff, 0xff, 0xff, 0xff};

    m_crgcap->TargetWriteDebug(base_crgcapreg + TRegCrgErrReg::CRGERRADDR_OFFSET, new uint8_t[4] {0xfe, 0xca, 0xfe, 0xca}, SIZE_32BIT);
    m_crgcap->TargetWriteDebug(base_crgcapreg + TRegCrgErrReg::CRGERRSTAT_OFFSET, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crgcap->TargetWriteDebug(base_crgcapreg + TRegCrgErrReg::CRGERRTYPE_OFFSET, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_crgcap->TargetWriteDebug(base_crgcapreg + TRegCrgErrReg::CRGERRCLR_OFFSET,  new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);

    m_crgcap->TargetReadDebug(base_crgcapreg + TRegCrgErrReg::CRGERRADDR_OFFSET, data_8_output_crgaddr, SIZE_32BIT);
    m_crgcap->TargetReadDebug(base_crgcapreg + TRegCrgErrReg::CRGERRSTAT_OFFSET, data_8_output_crgstat, SIZE_32BIT);
    m_crgcap->TargetReadDebug(base_crgcapreg + TRegCrgErrReg::CRGERRTYPE_OFFSET, data_8_output_crgtype, SIZE_32BIT);
    m_crgcap->TargetReadDebug(base_crgcapreg + TRegCrgErrReg::CRGERRCLR_OFFSET, data_8_output_crgclr, SIZE_32BIT);

    memcpy (&m_data32, data_8_output_crgaddr, sizeof(m_data32));
    uint32_t m_data32_out_crgaddr = m_data32;
    memcpy (&m_data32, data_8_output_crgstat, sizeof(m_data32));
    uint32_t m_data32_out_crgstat = m_data32;
    memcpy (&m_data32, data_8_output_crgtype, sizeof(m_data32));
    uint32_t m_data32_out_crgtype = m_data32;
    memcpy (&m_data32, data_8_output_crgclr, sizeof(m_data32));
    uint32_t m_data32_out_crgclr = m_data32;

    // Defined WriteBody function
    ASSERT_EQ(0x0U, m_data32_out_crgclr);
    ASSERT_EQ(0x0U, m_data32_out_crgaddr);
    ASSERT_EQ(0x0U, m_data32_out_crgtype);
    ASSERT_EQ(0x0U, m_data32_out_crgstat);
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
