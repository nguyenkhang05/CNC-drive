/**
 * @file test_treg_gerrstat.cpp
 *
 * @date Created on: 2016
 * @author Author: Quang Lam <quang.lam.px@rvc.renesas.com>
 * @brief UNIT TEST for GERRSTAT. (SAMPLE)
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

class GERRSTAT;

// ----------------------------------------
class TRegGERRSTAT1 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t>> {
protected:
    TRegGERRSTAT1() {}
    virtual ~TRegGERRSTAT1() {}
    virtual void SetUp() {
        m_treg_gerrstat   = new GERRSTAT (reg_size_gerrstat, min_access_size_gerrstat, m_reg_addr, "GERRSTAT", CORE_G4MH, CORE_VER_11);
    }
    virtual void TearDown() { delete m_treg_gerrstat; }

public:
    GERRSTAT   *m_treg_gerrstat;
    MockSideBand sideband;

public:
    uint32_t reg_size_gerrstat = SIZE_32BIT;
    uint32_t min_access_size_gerrstat = SIZE_32BIT;
    PhysAddr m_reg_addr = 0xFFFFFFFF;
};


/*!
 * @brief Check Initial method.
 */

TEST_F(TRegGERRSTAT1, Check_Initial_Setup) {
    ASSERT_STREQ("GERRSTAT", m_treg_gerrstat->GetName());
    uint8_t data[4] = {0x01, 0x01, 0x01, 0x01};
    uint32_t init_val = 0x00000000U;
    m_treg_gerrstat->ReadBody(m_reg_addr, data, SIZE_32BIT);
    uint32_t *wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(init_val, *wdata);
}


/*!
 * @brief Check Reset method.
 */
TEST_F(TRegGERRSTAT1, Check_Reset) {
    m_treg_gerrstat->Reset();
    uint8_t data[4] = {0x01, 0x01, 0x01, 0x01};
    uint32_t init_val = 0x00000000U;
    m_treg_gerrstat->ReadBody(m_reg_addr, data, SIZE_32BIT);
    uint32_t *wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(init_val, *wdata);
}


/*!
 * @brief Check Write method.
 */

TEST_P(TRegGERRSTAT1, Check_Write) {
    SideBand cur_sideband = sideband;
    PhysAddr cur_reg_addr = std::get<0>(GetParam());
    uint8_t *data     = std::get<1>(GetParam());
    ByteSize size     = std::get<2>(GetParam());

    if (size == SIZE_32BIT) {
        EXPECT_EQ(SYSERR_NONE, m_treg_gerrstat->Write(cur_sideband, cur_reg_addr, data, size));
    } else {
        ASSERT_THROW(m_treg_gerrstat->Write(cur_sideband, cur_reg_addr, data, size), std::range_error);
    }
}

std::tuple<PhysAddr, uint8_t *, uint32_t> const GetSet_ParametersTable[] = {
    // INPUT_DATA of GERRSTAT(SideBand, PhysAddr, uint8_t *, ByteSize=)
    /* 0000 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT),
    /* 0001 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT),
    /* 0002 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT),
    /* 0003 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_48BIT),
    /* 0004 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_64BIT),
    /* 0005 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_128BIT),
    // please write more test case.
};

INSTANTIATE_TEST_CASE_P(Test_Write_WithParameter, TRegGERRSTAT1,
                        ::testing::ValuesIn(GetSet_ParametersTable));


/*!
 * @brief Check OVF value.
 */

TEST_F(TRegGERRSTAT1, Check_OVF_val) {
    RegData ovf_val = 0xA5A5A5A5U;
    m_treg_gerrstat->SetOVF(ovf_val);
    EXPECT_EQ(0x01U, m_treg_gerrstat->GetOVF());
    ovf_val = 0xCAFECAFEU;
    m_treg_gerrstat->SetOVF(ovf_val);
    EXPECT_EQ(0x00U, m_treg_gerrstat->GetOVF());
}


/*!
 * @brief Check CLR value.
 */

TEST_F(TRegGERRSTAT1, Check_CLR_val) {
    RegData err_val = 0xA5A5A5A5U;
    m_treg_gerrstat->SetERR(err_val);
    EXPECT_EQ(0x01U, m_treg_gerrstat->GetERR());
    err_val = 0xCAFECAFEU;
    m_treg_gerrstat->SetERR(err_val);
    EXPECT_EQ(0x00U, m_treg_gerrstat->GetERR());
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
