/**
 * @file test_treg_gerrtype.cpp
 *
 * @date Created on: 2016
 * @author Author: Quang Lam <quang.lam.px@rvc.renesas.com>
 * @brief UNIT TEST for GERRTYPE. (SAMPLE)
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

class GERRTYPE;

// ----------------------------------------
class TRegGERRTYPE1 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t>> {
protected:
    TRegGERRTYPE1() {}
    virtual ~TRegGERRTYPE1() {}
    virtual void SetUp() {
        m_treg_gerrtype   = new GERRTYPE (reg_size_gerrtype, min_access_size_gerrtype, m_reg_addr, "GERRTYPE");
    }
    virtual void TearDown() { delete m_treg_gerrtype; }

public:
    GERRTYPE   *m_treg_gerrtype;
    MockSideBand sideband;

public:
    uint32_t reg_size_gerrtype = SIZE_32BIT;
    uint32_t min_access_size_gerrtype = SIZE_32BIT;
    PhysAddr m_reg_addr = 0xFFFFFFFF;
};


/*!
 * @brief Check Initial method.
 */

TEST_F(TRegGERRTYPE1, Check_Initial_Setup) {
    ASSERT_STREQ("GERRTYPE", m_treg_gerrtype->GetName());
    uint8_t data[4] = {0x01, 0x01, 0x01, 0x01};
    uint32_t init_val = 0x00000000U;
    m_treg_gerrtype->ReadBody(m_reg_addr, data, SIZE_32BIT);
    uint32_t *wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(init_val, *wdata);
}


/*!
 * @brief Check Reset method.
 */
TEST_F(TRegGERRTYPE1, Check_Reset) {
    m_treg_gerrtype->Reset();
    uint8_t data[4] = {0x01, 0x01, 0x01, 0x01};
    uint32_t init_val = 0x00000000U;
    m_treg_gerrtype->ReadBody(m_reg_addr, data, SIZE_32BIT);
    uint32_t *wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(init_val, *wdata);
}


/*!
 * @brief Check Write method.
 */

TEST_P(TRegGERRTYPE1, Check_Write) {
    SideBand cur_sideband = sideband;
    PhysAddr cur_reg_addr = std::get<0>(GetParam());
    uint8_t *data     = std::get<1>(GetParam());
    ByteSize size     = std::get<2>(GetParam());

    if (size == SIZE_32BIT) {
        EXPECT_EQ(SYSERR_NONE, m_treg_gerrtype->Write(cur_sideband, cur_reg_addr, data, size));
    } else {
        ASSERT_THROW(m_treg_gerrtype->Write(cur_sideband, cur_reg_addr, data, size), std::range_error);
    }
}

std::tuple<PhysAddr, uint8_t *, uint32_t> const GetSet_ParametersTable[] = {
    // INPUT_DATA of GERRTYPE(SideBand, PhysAddr, uint8_t *, ByteSize=)
    /* 0000 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT),
    /* 0001 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT),
    /* 0002 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT),
    /* 0003 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_48BIT),
    /* 0004 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_64BIT),
    /* 0005 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_128BIT),
    // please write more test case.
};

INSTANTIATE_TEST_CASE_P(Test_Write_WithParameter, TRegGERRTYPE1,
                        ::testing::ValuesIn(GetSet_ParametersTable));


/*!
 * @brief Check SEC value.
 */

TEST_F(TRegGERRTYPE1, Check_SEC_val) {
    RegData sec_val = 0xA5A5A5A5U;
    m_treg_gerrtype->SetSEC(sec_val);
    EXPECT_EQ(0x01U, m_treg_gerrtype->GetSEC());
    sec_val = 0xCAFECAFEU;
    m_treg_gerrtype->SetSEC(sec_val);
    EXPECT_EQ(0x00U, m_treg_gerrtype->GetSEC());
}


/*!
 * @brief Check DBG value.
 */

TEST_F(TRegGERRTYPE1, Check_DBG_val) {
    RegData dbg_val = 0xA5A5A5A5U;
    m_treg_gerrtype->SetDBG(dbg_val);
    EXPECT_EQ(0x01U, m_treg_gerrtype->GetDBG());
    dbg_val = 0xCAFECAFEU;
    m_treg_gerrtype->SetDBG(dbg_val);
    EXPECT_EQ(0x00U, m_treg_gerrtype->GetDBG());
}


/*!
 * @brief Check UM value.
 */

TEST_F(TRegGERRTYPE1, Check_UM_val) {
    RegData um_val = 0xA5A5A5A5U;
    m_treg_gerrtype->SetUM(um_val);
    EXPECT_EQ(0x01U, m_treg_gerrtype->GetUM());
    um_val = 0xCAFECAFEU;
    m_treg_gerrtype->SetUM(um_val);
    EXPECT_EQ(0x00U, m_treg_gerrtype->GetUM());
}


/*!
 * @brief Check Write value.
 */

TEST_F(TRegGERRTYPE1, Check_Write_val) {
    RegData write_val = 0xA5A5A5A5U;
    m_treg_gerrtype->SetWrite(write_val);
    EXPECT_EQ(0x01U, m_treg_gerrtype->GetWrite());
    write_val = 0xCAFECAFEU;
    m_treg_gerrtype->SetWrite(write_val);
    EXPECT_EQ(0x00U, m_treg_gerrtype->GetWrite());
}


/*!
 * @brief Check SPID value.
 */

TEST_F(TRegGERRTYPE1, Check_SPID_val) {
    RegData spid_val = 0xA5A5A5A5U;
    m_treg_gerrtype->SetSPID(spid_val);
    EXPECT_EQ(0x05U, m_treg_gerrtype->GetSPID());
    spid_val = 0xCAFECAFEU;
    m_treg_gerrtype->SetSPID(spid_val);
    EXPECT_EQ(0x1EU, m_treg_gerrtype->GetSPID());
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
