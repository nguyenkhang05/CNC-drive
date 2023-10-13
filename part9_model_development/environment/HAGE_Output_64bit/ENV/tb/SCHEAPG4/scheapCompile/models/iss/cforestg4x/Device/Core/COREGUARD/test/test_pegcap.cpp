/**
 * @file test_pegcap.cpp
 *
 * @date Created on: 2016
 * @author Author: Quang Lam <quang.lam.px@rvc.renesas.com>
 * @brief UNIT TEST for behavior PEG Capture. (SAMPLE)
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

class PegCap;

// ----------------------------------------
class PegCap1 : public testing::Test {
protected:
    PegCap1() {}
    virtual ~PegCap1() {}
    virtual void SetUp() {
        m_pegcap           = new PegCap (m_peid, base_pegcapreg, CORE_G4MH, CORE_VER_11);
    }
    virtual void TearDown() { delete m_pegcap; }

public:
    PegCap          *m_pegcap;
    MockSideBand sideband;

public:
    uint32_t m_peid = 0x0;
    uint32_t base_pegcapreg = 0xFFC6C800;
    union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };
};


/*!
 * @brief Check Initial method.
 */

TEST_F(PegCap1, Check_Initial_Setup) {
    ASSERT_NE(nullptr, m_pegcap->GetPegErrReg());
}


/*!
 * @brief Check Reset method.
 */
TEST_F(PegCap1, Check_Reset) {
    SideBand sb_info;
    // Change addr value in PEGADDR to confirm if it is reset or not.
    m_pegcap->GetPegErrReg()->GetPegErrAddr()->SetAddr(0xcafecafe);
    m_pegcap->HardReset();
    ASSERT_EQ(0x0U, m_pegcap->GetPegErrReg()->GetPegErrAddr()->GetAddr());
}


/*!
 * @brief Check TargetWrite method.
 */
TEST_F(PegCap1, Check_TargetWrite) {
    SideBand sb_info;
    ASSERT_THROW(m_pegcap->TargetWrite(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRADDR_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_pegcap->TargetWrite(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRSTAT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_pegcap->TargetWrite(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRTYPE_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);
    ASSERT_THROW(m_pegcap->TargetWrite(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRCLR_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_64BIT), std::range_error);

    ASSERT_EQ(SYSERR_NONE, m_pegcap->TargetWrite(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRADDR_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT));
    ASSERT_EQ(SYSERR_NONE, m_pegcap->TargetWrite(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRSTAT_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT));
    ASSERT_EQ(SYSERR_NONE, m_pegcap->TargetWrite(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRTYPE_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT));
    ASSERT_EQ(SYSERR_NONE, m_pegcap->TargetWrite(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRCLR_OFFSET, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT));
}


/*!
 * @brief Check TargetRead method.
 */
TEST_F(PegCap1, Check_TargetRead) {
    SideBand sb_info;

    uint8_t data_8_output_pegaddr[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegstat[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegtype[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegclr [4]   = {0xff, 0xff, 0xff, 0xff};

    m_pegcap->GetPegErrReg()->GetPegErrAddr()->SetAddr(0xcafecafe);
    m_pegcap->GetPegErrReg()->GetPegErrStat()->SetERR(0x1);
    m_pegcap->GetPegErrReg()->GetPegErrType()->SetWrite(0x1);


    m_pegcap->TargetRead(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRADDR_OFFSET, data_8_output_pegaddr, SIZE_32BIT);
    m_pegcap->TargetRead(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRSTAT_OFFSET, data_8_output_pegstat, SIZE_32BIT);
    m_pegcap->TargetRead(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRTYPE_OFFSET, data_8_output_pegtype, SIZE_32BIT);
    m_pegcap->TargetRead(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRCLR_OFFSET,  data_8_output_pegclr, SIZE_32BIT);

    memcpy (&m_data32, data_8_output_pegaddr, sizeof(m_data32));
    uint32_t m_data32_out_pegaddr = m_data32;
    memcpy (&m_data32, data_8_output_pegstat, sizeof(m_data32));
    uint32_t m_data32_out_pegstat = m_data32;
    memcpy (&m_data32, data_8_output_pegtype, sizeof(m_data32));
    uint32_t m_data32_out_pegtype = m_data32;
    memcpy (&m_data32, data_8_output_pegclr, sizeof(m_data32));
    uint32_t m_data32_out_pegclr = m_data32;

    ASSERT_EQ(0xcafecafe, m_data32_out_pegaddr);
    ASSERT_EQ(0x1U, m_data32_out_pegstat);
    ASSERT_EQ(0x1U, m_data32_out_pegtype);
    ASSERT_EQ(0x0U, m_data32_out_pegclr);
}


/*!
 * @brief Check TargetFetch method.
 */
TEST_F(PegCap1, Check_TargetFetch) {
    SideBand sb_info;

    uint8_t data_8_output_pegaddr[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegstat[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegtype[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegclr [4]   = {0xff, 0xff, 0xff, 0xff};

    m_pegcap->GetPegErrReg()->GetPegErrAddr()->SetAddr(0xcafecafe);
    m_pegcap->GetPegErrReg()->GetPegErrStat()->SetERR(0x1);
    m_pegcap->GetPegErrReg()->GetPegErrType()->SetWrite(0x1);


    m_pegcap->TargetFetch(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRADDR_OFFSET, data_8_output_pegaddr, SIZE_32BIT);
    m_pegcap->TargetFetch(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRSTAT_OFFSET, data_8_output_pegstat, SIZE_32BIT);
    m_pegcap->TargetFetch(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRTYPE_OFFSET, data_8_output_pegtype, SIZE_32BIT);
    m_pegcap->TargetFetch(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRCLR_OFFSET,  data_8_output_pegclr, SIZE_32BIT);

    memcpy (&m_data32, data_8_output_pegaddr, sizeof(m_data32));
    uint32_t m_data32_out_pegaddr = m_data32;
    memcpy (&m_data32, data_8_output_pegstat, sizeof(m_data32));
    uint32_t m_data32_out_pegstat = m_data32;
    memcpy (&m_data32, data_8_output_pegtype, sizeof(m_data32));
    uint32_t m_data32_out_pegtype = m_data32;
    memcpy (&m_data32, data_8_output_pegclr, sizeof(m_data32));
    uint32_t m_data32_out_pegclr = m_data32;

    ASSERT_EQ(0xcafecafe, m_data32_out_pegaddr);
    ASSERT_EQ(0x1U, m_data32_out_pegstat);
    ASSERT_EQ(0x1U, m_data32_out_pegtype);
    ASSERT_EQ(0x0U, m_data32_out_pegclr);
}


/*!
 * @brief Check TargetWriteDebug method.
 */
TEST_F(PegCap1, Check_TargetWriteDebug) {
    SideBand sb_info;
    uint8_t data_8_output_pegaddr[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegstat[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegtype[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegclr [4]   = {0xff, 0xff, 0xff, 0xff};

    m_pegcap->TargetWriteDebug(base_pegcapreg + TRegPegErrReg::PEGERRADDR_OFFSET, new uint8_t[4] {0xfe, 0xca, 0xfe, 0xca}, SIZE_32BIT);
    m_pegcap->TargetWriteDebug(base_pegcapreg + TRegPegErrReg::PEGERRSTAT_OFFSET, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_pegcap->TargetWriteDebug(base_pegcapreg + TRegPegErrReg::PEGERRTYPE_OFFSET, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_pegcap->TargetWriteDebug(base_pegcapreg + TRegPegErrReg::PEGERRCLR_OFFSET,  new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);

    m_pegcap->TargetRead(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRADDR_OFFSET, data_8_output_pegaddr, SIZE_32BIT);
    m_pegcap->TargetRead(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRSTAT_OFFSET, data_8_output_pegstat, SIZE_32BIT);
    m_pegcap->TargetRead(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRTYPE_OFFSET, data_8_output_pegtype, SIZE_32BIT);
    m_pegcap->TargetRead(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRCLR_OFFSET,  data_8_output_pegclr, SIZE_32BIT);

    memcpy (&m_data32, data_8_output_pegaddr, sizeof(m_data32));
    uint32_t m_data32_out_pegaddr = m_data32;
    memcpy (&m_data32, data_8_output_pegstat, sizeof(m_data32));
    uint32_t m_data32_out_pegstat = m_data32;
    memcpy (&m_data32, data_8_output_pegtype, sizeof(m_data32));
    uint32_t m_data32_out_pegtype = m_data32;
    memcpy (&m_data32, data_8_output_pegclr, sizeof(m_data32));
    uint32_t m_data32_out_pegclr = m_data32;

    // Defined WriteBody function
    ASSERT_EQ(0x0U, m_data32_out_pegclr);

    // Didn't define WriteBody function, so, the read value is always 0.
    ASSERT_EQ(0x0U, m_data32_out_pegaddr);
    ASSERT_EQ(0x0U, m_data32_out_pegtype);
    ASSERT_EQ(0x0U, m_data32_out_pegstat);

    m_pegcap->TargetWriteDebug(base_pegcapreg + TRegPegErrReg::PEGERRADDR_OFFSET, new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    m_pegcap->TargetWriteDebug(base_pegcapreg + TRegPegErrReg::PEGERRSTAT_OFFSET, new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    m_pegcap->TargetWriteDebug(base_pegcapreg + TRegPegErrReg::PEGERRTYPE_OFFSET, new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);
    m_pegcap->TargetWriteDebug(base_pegcapreg + TRegPegErrReg::PEGERRCLR_OFFSET,  new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT);

    m_pegcap->TargetRead(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRADDR_OFFSET, data_8_output_pegaddr, SIZE_32BIT);
    m_pegcap->TargetRead(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRSTAT_OFFSET, data_8_output_pegstat, SIZE_32BIT);
    m_pegcap->TargetRead(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRTYPE_OFFSET, data_8_output_pegtype, SIZE_32BIT);
    m_pegcap->TargetRead(sb_info, base_pegcapreg + TRegPegErrReg::PEGERRCLR_OFFSET,  data_8_output_pegclr, SIZE_32BIT);

    memcpy (&m_data32, data_8_output_pegaddr, sizeof(m_data32));
    m_data32_out_pegaddr = m_data32;
    memcpy (&m_data32, data_8_output_pegstat, sizeof(m_data32));
    m_data32_out_pegstat = m_data32;
    memcpy (&m_data32, data_8_output_pegtype, sizeof(m_data32));
    m_data32_out_pegtype = m_data32;
    memcpy (&m_data32, data_8_output_pegclr, sizeof(m_data32));
    m_data32_out_pegclr = m_data32;

    // Defined WriteBody function
    ASSERT_EQ(0x0U, m_data32_out_pegclr);
    ASSERT_EQ(0x0U, m_data32_out_pegaddr);
    ASSERT_EQ(0x0U, m_data32_out_pegtype);
    ASSERT_EQ(0x0U, m_data32_out_pegstat);
}


/*!
 * @brief Check TargetReadDebug method.
 */
TEST_F(PegCap1, Check_TargetReadDebug) {
    SideBand sb_info;
    uint8_t data_8_output_pegaddr[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegstat[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegtype[4]   = {0xff, 0xff, 0xff, 0xff};
    uint8_t data_8_output_pegclr [4]   = {0xff, 0xff, 0xff, 0xff};

    m_pegcap->TargetWriteDebug(base_pegcapreg + TRegPegErrReg::PEGERRADDR_OFFSET, new uint8_t[4] {0xfe, 0xca, 0xfe, 0xca}, SIZE_32BIT);
    m_pegcap->TargetWriteDebug(base_pegcapreg + TRegPegErrReg::PEGERRSTAT_OFFSET, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_pegcap->TargetWriteDebug(base_pegcapreg + TRegPegErrReg::PEGERRTYPE_OFFSET, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);
    m_pegcap->TargetWriteDebug(base_pegcapreg + TRegPegErrReg::PEGERRCLR_OFFSET,  new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT);

    m_pegcap->TargetReadDebug(base_pegcapreg + TRegPegErrReg::PEGERRADDR_OFFSET, data_8_output_pegaddr, SIZE_32BIT);
    m_pegcap->TargetReadDebug(base_pegcapreg + TRegPegErrReg::PEGERRSTAT_OFFSET, data_8_output_pegstat, SIZE_32BIT);
    m_pegcap->TargetReadDebug(base_pegcapreg + TRegPegErrReg::PEGERRTYPE_OFFSET, data_8_output_pegtype, SIZE_32BIT);
    m_pegcap->TargetReadDebug(base_pegcapreg + TRegPegErrReg::PEGERRCLR_OFFSET, data_8_output_pegclr, SIZE_32BIT);

    memcpy (&m_data32, data_8_output_pegaddr, sizeof(m_data32));
    uint32_t m_data32_out_pegaddr = m_data32;
    memcpy (&m_data32, data_8_output_pegstat, sizeof(m_data32));
    uint32_t m_data32_out_pegstat = m_data32;
    memcpy (&m_data32, data_8_output_pegtype, sizeof(m_data32));
    uint32_t m_data32_out_pegtype = m_data32;
    memcpy (&m_data32, data_8_output_pegclr, sizeof(m_data32));
    uint32_t m_data32_out_pegclr = m_data32;

    // Defined WriteBody function
    ASSERT_EQ(0x0U, m_data32_out_pegclr);
    ASSERT_EQ(0x0U, m_data32_out_pegaddr);
    ASSERT_EQ(0x0U, m_data32_out_pegtype);
    ASSERT_EQ(0x0U, m_data32_out_pegstat);
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
