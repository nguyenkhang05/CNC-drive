/**
 * @file test_guard_internal.cpp
 *
 * @date Created on: 2016
 * @author Author: Quang Lam <quang.lam.px@rvc.renesas.com>
 * @brief UNIT TEST for GKCSPID. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include <memory>
#include <map>
#include <stdexcept>
#include "treg_guard.h"
#include "mock_sideband.h"
#include "gmock/gmock.h"

// using ::testing::AtLeast;
// using ::testing::_;
// using ::testing::Return;

class GuardInternal;

// ----------------------------------------
class GuardInternal1 : public testing::Test {
protected:
    GuardInternal1() {}
    virtual ~GuardInternal1() {}
    virtual void SetUp() {
        m_guard_internal= new GuardInternal(false, false, 0x0, 0x8, 0x4, GUARD_INTERNAL_CHANNEL, 0x0, 0x0);
    }
    virtual void TearDown() {
        delete m_guard_internal;
    }

public:
    static const uint32_t GUARD_INTERNAL_CHANNEL = 0x8;
    GuardInternal* m_guard_internal;
};


/*!
 * @brief Check Reset method.
 */
TEST_F(GuardInternal1, Check_Reset) {
    m_guard_internal->Reset();
    EXPECT_FALSE(m_guard_internal->IsGuardEn());
    EXPECT_FALSE(m_guard_internal->IsWriteEn());
    for (uint32_t chn = 0; chn < GUARD_INTERNAL_CHANNEL; chn++) {
        EXPECT_FALSE(m_guard_internal->IsChannelEn(chn));
    }
}


/*!
 * @brief Check IsGuardEn value.
 */

TEST_F(GuardInternal1, Check_IsGuardEn) {
    m_guard_internal->SetGuardEn(true);
    EXPECT_TRUE(m_guard_internal->IsGuardEn());
    m_guard_internal->SetGuardEn(false);
    EXPECT_FALSE(m_guard_internal->IsGuardEn());
}


/*!
 * @brief Check IsWriteEn value.
 */

TEST_F(GuardInternal1, Check_IsWriteEn) {
    m_guard_internal->SetWriteEn(true);
    EXPECT_TRUE(m_guard_internal->IsWriteEn());
    m_guard_internal->SetWriteEn(false);
    EXPECT_FALSE(m_guard_internal->IsWriteEn());
}


/*!
 * @brief Check IsChannelEn value.
 */

TEST_F(GuardInternal1, Check_IsChannelEn) {
    m_guard_internal->SetChannelEn(0x0, true);
    EXPECT_TRUE(m_guard_internal->IsChannelEn(0x0));
    m_guard_internal->SetChannelEn(0x1, true);
    EXPECT_TRUE(m_guard_internal->IsChannelEn(0x1));
    m_guard_internal->SetChannelEn(0x2, true);
    EXPECT_TRUE(m_guard_internal->IsChannelEn(0x2));
    m_guard_internal->SetChannelEn(0x3, true);
    EXPECT_TRUE(m_guard_internal->IsChannelEn(0x3));
    m_guard_internal->SetChannelEn(0x4, true);
    EXPECT_TRUE(m_guard_internal->IsChannelEn(0x4));
    m_guard_internal->SetChannelEn(0x5, true);
    EXPECT_TRUE(m_guard_internal->IsChannelEn(0x5));
    m_guard_internal->SetChannelEn(0x6, true);
    EXPECT_TRUE(m_guard_internal->IsChannelEn(0x6));
    m_guard_internal->SetChannelEn(0x7, true);
    EXPECT_TRUE(m_guard_internal->IsChannelEn(0x7));

    m_guard_internal->SetChannelEn(0x0, false);
    EXPECT_FALSE(m_guard_internal->IsChannelEn(0x0));
    m_guard_internal->SetChannelEn(0x1, false);
    EXPECT_FALSE(m_guard_internal->IsChannelEn(0x1));
    m_guard_internal->SetChannelEn(0x2, false);
    EXPECT_FALSE(m_guard_internal->IsChannelEn(0x2));
    m_guard_internal->SetChannelEn(0x3, false);
    EXPECT_FALSE(m_guard_internal->IsChannelEn(0x3));
    m_guard_internal->SetChannelEn(0x4, false);
    EXPECT_FALSE(m_guard_internal->IsChannelEn(0x4));
    m_guard_internal->SetChannelEn(0x5, false);
    EXPECT_FALSE(m_guard_internal->IsChannelEn(0x5));
    m_guard_internal->SetChannelEn(0x6, false);
    EXPECT_FALSE(m_guard_internal->IsChannelEn(0x6));
    m_guard_internal->SetChannelEn(0x7, false);
    EXPECT_FALSE(m_guard_internal->IsChannelEn(0x7));
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
