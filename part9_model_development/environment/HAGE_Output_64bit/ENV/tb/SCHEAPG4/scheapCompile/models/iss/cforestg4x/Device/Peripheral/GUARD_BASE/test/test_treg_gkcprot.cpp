/**
 * @file test_treg_gkcprot.cpp
 *
 * @date Created on: 2016
 * @author Author: Quang Lam <quang.lam.px@rvc.renesas.com>
 * @brief UNIT TEST for GKCPROT. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include <memory>
#include <map>
#include <stdexcept>
#include "treg_guard.h"
#include "mock_sideband.h"
#include "mock_guard_internal.h"
#include "gmock/gmock.h"

 using ::testing::AtLeast;
 using ::testing::_;
// using ::testing::Return;

class GKCPROT;

// ----------------------------------------
class TRegGKCPROT1 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t>> {
protected:
    TRegGKCPROT1() {}
    virtual ~TRegGKCPROT1() {}
    virtual void SetUp() {
        m_treg_gkcprot = new GKCPROT(reg_size, min_access_size, reg_addr, "GKCPROT", &m_guard_internal);
    }
    virtual void TearDown() { delete m_treg_gkcprot; }

public:
    MockGuardInternal m_guard_internal;
    GKCPROT *m_treg_gkcprot;
    MockSideBand sideband;

private:
    uint32_t reg_size = SIZE_32BIT;
    uint32_t min_access_size = SIZE_32BIT;
    PhysAddr reg_addr = 0xFFFFFFFF;
};

/*!
 * @brief Check Initial method.
 */
TEST_F(TRegGKCPROT1, Check_Initial_Setup) {
    ASSERT_STREQ("GKCPROT", m_treg_gkcprot->GetName());
    EXPECT_EQ(0x0U, m_treg_gkcprot->GetKCE());
}

/*!
 * @brief Check Reset method.
 */
TEST_F(TRegGKCPROT1, Check_Reset) {
    m_treg_gkcprot->Reset();
    EXPECT_EQ(0x0U, m_treg_gkcprot->GetKCE());
}


/*!
 * @brief Check Write method.
 */

TEST_P(TRegGKCPROT1, Check_Write) {
    {
        SideBand cur_sideband = sideband;
        PhysAddr cur_reg_addr = std::get<0>(GetParam());
        uint8_t *data     = new uint8_t(0xFF);
        ByteSize size     = std::get<2>(GetParam());
        if (size == SIZE_32BIT) {
            EXPECT_EQ(SYSERR_NONE, m_treg_gkcprot->Write(cur_sideband, cur_reg_addr, data, size));
        } else {
            ASSERT_THROW(m_treg_gkcprot->Write(cur_sideband, cur_reg_addr, data, size), std::range_error);
        }
    }
}

std::tuple<PhysAddr, uint8_t *, uint32_t> const GetSet_ParametersTable[] = {
    // INPUT_DATA of GKCPROT (SideBand, PhysAddr, uint8_t *, ByteSize=)
    /* 0000 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT),
    /* 0001 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT),
    /* 0002 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT),
    /* 0003 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_48BIT),
    /* 0004 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_64BIT),
    /* 0005 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_128BIT),
    // please write more test case.
};

INSTANTIATE_TEST_CASE_P(Test_Write_WithParameter, TRegGKCPROT1,
                        ::testing::ValuesIn(GetSet_ParametersTable));




// ----------------------------------------
class TRegGKCPROT2 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t, bool>> {
protected:
	TRegGKCPROT2() {}
    virtual ~TRegGKCPROT2() {}
    virtual void SetUp() {
        m_treg_gkcprot = new GKCPROT(reg_size, min_access_size, reg_addr, "GKCPROT", &m_guard_internal);
    }
    virtual void TearDown() { delete m_treg_gkcprot; }

public:
    MockGuardInternal m_guard_internal;
    GKCPROT *m_treg_gkcprot;
    MockSideBand sideband;

private:
    uint32_t reg_size = SIZE_32BIT;
    uint32_t min_access_size = SIZE_32BIT;
    PhysAddr reg_addr = 0xFFFFFFFF;
};

/*!
 * @brief Check WriteBody method.
 */

TEST_P(TRegGKCPROT2, Check_WriteBody) {
    {
        SideBand cur_sideband = sideband;
        PhysAddr cur_reg_addr = std::get<0>(GetParam());
        uint8_t *data     = std::get<1>(GetParam());
        ByteSize size     = std::get<2>(GetParam());
        bool is_write     = std::get<3>(GetParam());

        // Clear KCE at the beginning.
        m_treg_gkcprot->SetKCE(0x0);
        // In GKCPROT, WriteBody method can access by TargetWriteDebug function in case access size is not 32BIT.
        // So, UT must guarantee the behavior in case invalid access.
        if (size != SIZE_32BIT) {
            ASSERT_THROW(m_treg_gkcprot->WriteBody(cur_reg_addr, data, size), std::range_error);
        } else {
            if (is_write) {
                if (data[0] == 0x0U) { // It is written in the same conditions in order to improve the maintainability.
                    EXPECT_CALL(m_guard_internal, SetWriteEn(false))
                      .Times(AtLeast(1));
                }
                if (data[0] == 0x1U) { // It is written in the same conditions in order to improve the maintainability.
                    EXPECT_CALL(m_guard_internal, SetWriteEn(true))
                      .Times(AtLeast(1));
                }
            } else {
                EXPECT_CALL(m_guard_internal, SetWriteEn(_))
                  .Times(0);
            }


            EXPECT_EQ(SYSERR_NONE, m_treg_gkcprot->WriteBody(cur_reg_addr, data, size));
            if (is_write) {
                EXPECT_EQ(data[0], m_treg_gkcprot->GetKCE());
            } else {
                EXPECT_NE(data[0], m_treg_gkcprot->GetKCE());
            }
        }
    }
}

std::tuple<PhysAddr, uint8_t *, uint32_t, bool> const GetSet_ParametersTable2[] = {
    // INPUT_DATA of GKCPROT (PhysAddr, uint8_t *, ByteSize=)
    /* 0000 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0x00, 0xA5, 0xA5, 0xA5}, SIZE_32BIT, true),
    /* 0001 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0x01, 0xA5, 0xA5, 0xA5}, SIZE_32BIT, true),
    /* 0002 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0x01, 0xA5, 0x00, 0x01}, SIZE_32BIT, false),
    /* 0003 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0x00, 0xA5, 0xA5, 0xA5}, SIZE_8BIT, false),
    /* 0004 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0x00, 0xA5, 0xA5, 0xA5}, SIZE_16BIT, false),
    /* 0005 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0x00, 0xA5, 0xA5, 0xA5}, SIZE_64BIT, false),
    /* 0006 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0x00, 0xA5, 0xA5, 0xA5}, SIZE_128BIT, false),
    // please write more test case.
};

INSTANTIATE_TEST_CASE_P(Test_WriteBody_WithParameter, TRegGKCPROT2,
                        ::testing::ValuesIn(GetSet_ParametersTable2));


/*!
 * @brief Check KCE value.
 */
TEST_F(TRegGKCPROT2, Check_KCE_val) {
    RegData kce_val = 0x1;
    m_treg_gkcprot->SetKCE(kce_val);
    EXPECT_EQ(kce_val, m_treg_gkcprot->GetKCE());
    kce_val = 0x0;
    m_treg_gkcprot->SetKCE(kce_val);
    EXPECT_EQ(kce_val, m_treg_gkcprot->GetKCE());
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
