/**
 * @file test_treg_gerraddr.cpp
 *
 * @date Created on: 2016
 * @author Author: Quang Lam <quang.lam.px@rvc.renesas.com>
 * @brief UNIT TEST for GERRADDR. (SAMPLE)
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

class GERRADDR;

// ----------------------------------------
class TRegGERRADDR1 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t>> {
protected:
    TRegGERRADDR1() {}
    virtual ~TRegGERRADDR1() {}
    virtual void SetUp() {
        m_treg_gerraddr   = new GERRADDR (reg_size_gerraddr, min_access_size_gerraddr, m_reg_addr, "GERRADDR");
    }
    virtual void TearDown() { delete m_treg_gerraddr; }

public:
    GERRADDR   *m_treg_gerraddr;
    MockSideBand sideband;

public:
    uint32_t reg_size_gerraddr = SIZE_32BIT;
    uint32_t min_access_size_gerraddr = SIZE_32BIT;
    PhysAddr m_reg_addr = 0xFFFFFFFF;
};


/*!
 * @brief Check Initial method.
 */

TEST_F(TRegGERRADDR1, Check_Initial_Setup) {
    ASSERT_STREQ("GERRADDR", m_treg_gerraddr->GetName());
    uint8_t data[4] = {0x01, 0x01, 0x01, 0x01};
    uint32_t init_val = 0x00000000U;
    m_treg_gerraddr->ReadBody(m_reg_addr, data, SIZE_32BIT);
    uint32_t *wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(init_val, *wdata);
}


/*!
 * @brief Check Reset method.
 */
TEST_F(TRegGERRADDR1, Check_Reset) {
    m_treg_gerraddr->Reset();
    uint8_t data[4] = {0x01, 0x01, 0x01, 0x01};
    uint32_t init_val = 0x00000000U;
    m_treg_gerraddr->ReadBody(m_reg_addr, data, SIZE_32BIT);
    uint32_t *wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(init_val, *wdata);
}


/*!
 * @brief Check Write method.
 */

TEST_P(TRegGERRADDR1, Check_Write) {
    SideBand cur_sideband = sideband;
    PhysAddr cur_reg_addr = std::get<0>(GetParam());
    uint8_t *data     = std::get<1>(GetParam());
    ByteSize size     = std::get<2>(GetParam());

    if (size == SIZE_32BIT) {
        EXPECT_EQ(SYSERR_NONE, m_treg_gerraddr->Write(cur_sideband, cur_reg_addr, data, size));
    } else {
        ASSERT_THROW(m_treg_gerraddr->Write(cur_sideband, cur_reg_addr, data, size), std::range_error);
    }
}

std::tuple<PhysAddr, uint8_t *, uint32_t> const GetSet_ParametersTable[] = {
    // INPUT_DATA of GERRADDR(SideBand, PhysAddr, uint8_t *, ByteSize=)
    /* 0000 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT),
    /* 0001 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT),
    /* 0002 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT),
    /* 0003 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_48BIT),
    /* 0004 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_64BIT),
    /* 0005 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_128BIT),
    // please write more test case.
};

INSTANTIATE_TEST_CASE_P(Test_Write_WithParameter, TRegGERRADDR1,
                        ::testing::ValuesIn(GetSet_ParametersTable));


/*!
 * @brief Check ADDR value.
 */

TEST_F(TRegGERRADDR1, Check_ADDR_val) {
    RegData addr_val = 0xA5A5A5A5;
    m_treg_gerraddr->SetAddr(addr_val);
    EXPECT_EQ(addr_val, m_treg_gerraddr->GetAddr());
    addr_val = 0xCAFECAFE;
    m_treg_gerraddr->SetAddr(addr_val);
    EXPECT_EQ(addr_val, m_treg_gerraddr->GetAddr());
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
