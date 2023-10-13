/**
 * @file test_treg_gprot_20.cpp
 *
 * @date Created on: 2016
 * @author Author: Hoanh Ho <hoanh.ho.px@renesas.com>
 * @brief UNIT TEST for GPROT. (SAMPLE)
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
// using ::testing::_;
// using ::testing::Return;

class GPROT;
class GuardInternal;

// ----------------------------------------
class TRegGPROT1 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t>> {
protected:
    TRegGPROT1() {}
    virtual ~TRegGPROT1() {}
    virtual void SetUp() {
        m_treg_gprot   = new GPROT (reg_size_gprot, min_access_size_gprot, m_reg_addr, m_channel, "GPROT", CRG_MODULE, CORE_G4MH, CORE_VER_20, &m_guard_internal);
    }
    virtual void TearDown() {
        delete m_treg_gprot;
    }

public:
    MockGuardInternal m_guard_internal;
    GPROT   *m_treg_gprot;
    MockSideBand sideband;

public:
    uint32_t reg_size_gprot = SIZE_32BIT;
    uint32_t min_access_size_gprot = SIZE_8BIT;
    PhysAddr m_reg_addr = 0xFFFFFFFF;
    uint32_t m_channel = 0x0;
};


/*!
 * @brief Check Initial method.
 */

TEST_F(TRegGPROT1, Check_Initial_Setup) {
    ASSERT_STREQ("GPROT", m_treg_gprot->GetName());
    uint8_t data[4] = {0x01, 0x01, 0x01, 0x01};
    uint32_t init_val = 0x00000000U;
    m_treg_gprot->ReadBody(m_reg_addr, data, SIZE_32BIT);
    uint32_t *wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(init_val, *wdata);
}


/*!
 * @brief Check Reset method.
 */
TEST_F(TRegGPROT1, Check_Reset) {
    m_treg_gprot->Reset();
    uint8_t data[4] = {0x01, 0x01, 0x01, 0x01};
    uint32_t init_val = 0x00000000U;
    m_treg_gprot->ReadBody(m_reg_addr, data, SIZE_32BIT);
    uint32_t *wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(init_val, *wdata);
}


/*!
 * @brief Check Write method.
 */

TEST_P(TRegGPROT1, Check_Write) {
    SideBand cur_sideband = sideband;
    PhysAddr cur_reg_addr = std::get<0>(GetParam());
    uint8_t *data     = std::get<1>(GetParam());
    ByteSize size     = std::get<2>(GetParam());

    EXPECT_CALL(m_guard_internal, IsWriteEn())
        .Times(AtLeast(1));
    m_guard_internal.IsWriteEn();
    if ((size == SIZE_8BIT) || (size == SIZE_16BIT) || (size == SIZE_32BIT)) {
        EXPECT_EQ(SYSERR_NONE, m_treg_gprot->Write(cur_sideband, cur_reg_addr, data, size));
        EXPECT_EQ(0x1U, m_treg_gprot->GetRG());
    } else {
        ASSERT_THROW(m_treg_gprot->Write(cur_sideband, cur_reg_addr, data, size), std::range_error);
    }
}

std::tuple<PhysAddr, uint8_t *, uint32_t> const GetSet_ParametersTable[] = {
    // INPUT_DATA of GPROT(SideBand, PhysAddr, uint8_t *, ByteSize=)
    /* 0000 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT),
    /* 0001 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT),
    /* 0002 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT),
    /* 0003 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_48BIT),
    /* 0004 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_64BIT),
    /* 0005 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_128BIT),
    // please write more test case.
};

INSTANTIATE_TEST_CASE_P(Test_Write_WithParameter, TRegGPROT1,
                        ::testing::ValuesIn(GetSet_ParametersTable));


// ----------------------------------------
class TRegGPROT2 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t, bool>> {
protected:
    TRegGPROT2() {}
    virtual ~TRegGPROT2() {}
    virtual void SetUp() {
        m_treg_gprot   = new GPROT (reg_size_gprot, min_access_size_gprot, m_reg_addr, m_channel, "GPROT", CRG_MODULE, CORE_G4MH, CORE_VER_20, &m_guard_internal);
    }
    virtual void TearDown() {
        delete m_treg_gprot;
    }

public:
    MockGuardInternal m_guard_internal;
    GPROT   *m_treg_gprot;
    MockSideBand sideband;

public:
    uint32_t reg_size_gprot = SIZE_32BIT;
    uint32_t min_access_size_gprot = SIZE_8BIT;
    PhysAddr m_reg_addr = 0xFFFFFFFF;
    uint32_t m_channel = 0x0;
};


/*!
 * @brief Check available write bit..
 */

TEST_F(TRegGPROT2, Check_EnWrite_val) {
    // Write all available bit to 1.
    EXPECT_EQ(SYSERR_NONE, m_treg_gprot->WriteBody(0x0000cafe, new uint8_t[4] {0x53, 0x01, 0x00, 0x00}, SIZE_32BIT));
    EXPECT_EQ(0x1U, m_treg_gprot->GetGEN());
    EXPECT_EQ(0x1U, m_treg_gprot->GetDBG());
    EXPECT_EQ(0x1U, m_treg_gprot->GetUME());
    EXPECT_EQ(0x1U, m_treg_gprot->GetWG());
    EXPECT_EQ(0x1U, m_treg_gprot->GetRG());

    // Write all available bit to 0.
    EXPECT_EQ(SYSERR_NONE, m_treg_gprot->WriteBody(0x0000cafe, new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT));
    EXPECT_EQ(0x0U, m_treg_gprot->GetGEN());
    EXPECT_EQ(0x0U, m_treg_gprot->GetDBG());
    EXPECT_EQ(0x0U, m_treg_gprot->GetUME());
    EXPECT_EQ(0x0U, m_treg_gprot->GetWG());
    EXPECT_EQ(0x0U, m_treg_gprot->GetRG());

    // Write all available bit to 0 with inverse value.
    EXPECT_EQ(SYSERR_NONE, m_treg_gprot->WriteBody(0x0000cafe, new uint8_t[4] {0xac, 0xfe, 0xff, 0xff}, SIZE_32BIT));
    EXPECT_EQ(0x0U, m_treg_gprot->GetGEN());
    EXPECT_EQ(0x0U, m_treg_gprot->GetDBG());
    EXPECT_EQ(0x0U, m_treg_gprot->GetUME());
    EXPECT_EQ(0x0U, m_treg_gprot->GetWG());
    EXPECT_EQ(0x0U, m_treg_gprot->GetRG());

    // Write all available bit to 1.
    EXPECT_EQ(SYSERR_NONE, m_treg_gprot->WriteBody(0x0000cafe, new uint8_t[4] {0xff, 0xff, 0xff, 0xff}, SIZE_32BIT));
    EXPECT_EQ(0x1U, m_treg_gprot->GetGEN());
    EXPECT_EQ(0x1U, m_treg_gprot->GetDBG());
    EXPECT_EQ(0x1U, m_treg_gprot->GetUME());
    EXPECT_EQ(0x1U, m_treg_gprot->GetWG());
    EXPECT_EQ(0x1U, m_treg_gprot->GetRG());
}


/*!
 * @brief Check WriteBody method.
 */

TEST_P(TRegGPROT2, Check_WriteBody) {
    {
        SideBand cur_sideband = sideband;
        PhysAddr cur_reg_addr = std::get<0>(GetParam());
        uint8_t *data     = std::get<1>(GetParam());
        ByteSize size     = std::get<2>(GetParam());
        bool is_write     = std::get<3>(GetParam());

        if ((size != SIZE_32BIT) && (size != SIZE_16BIT) && (size != SIZE_8BIT)) {
            ASSERT_THROW(m_treg_gprot->WriteBody(cur_reg_addr, data, size), std::range_error);
        } else {
            EXPECT_EQ(SYSERR_NONE, m_treg_gprot->WriteBody(cur_reg_addr, data, size));
            // GPROT has access size: 8bit, 16bit, 32bit.
            if (is_write) {
                if ((size == SIZE_32BIT) || (size == SIZE_16BIT)) {
                    EXPECT_EQ(0x1U, m_treg_gprot->GetGEN());
                    EXPECT_EQ(0x1U, m_treg_gprot->GetDBG());
                    EXPECT_EQ(0x1U, m_treg_gprot->GetUME());
                    EXPECT_EQ(0x1U, m_treg_gprot->GetWG());
                    EXPECT_EQ(0x1U, m_treg_gprot->GetRG());
                } else {
                    uint32_t addr_check = cur_reg_addr & 0x3;
                    if (addr_check == 0x0) {
                        EXPECT_EQ(0x1U, m_treg_gprot->GetDBG());
                        EXPECT_EQ(0x1U, m_treg_gprot->GetUME());
                        EXPECT_EQ(0x1U, m_treg_gprot->GetWG());
                        EXPECT_EQ(0x1U, m_treg_gprot->GetRG());
                    } else { // addr_check = 0x1.
                        EXPECT_EQ(0x1U, m_treg_gprot->GetGEN());
                    }
                }
            } else {
                EXPECT_EQ(0x0U, m_treg_gprot->GetGEN());
                EXPECT_EQ(0x0U, m_treg_gprot->GetDBG());
                EXPECT_EQ(0x0U, m_treg_gprot->GetUME());
                EXPECT_EQ(0x0U, m_treg_gprot->GetWG());
                EXPECT_EQ(0x0U, m_treg_gprot->GetRG());
            }
        }
    }
}

std::tuple<PhysAddr, uint8_t *, uint32_t, bool> const GetSet_ParametersTable2[] = {
    // INPUT_DATA of GPROT(PhysAddr, uint8_t *, ByteSize=)
    /* 0000 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0x53, 0x01, 0x00, 0x00}, SIZE_32BIT, true),
    /* 0001 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT, true),
    /* 0002 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT, true),
    /* 0003 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT, false),
    /* 0004 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, true),
    /* 0005 */ std::make_tuple(0x0000cafd, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, true),
    /* 0006 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, false),
    /* 0007 */ std::make_tuple(0x0000caff, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, false),
    /* 0008 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_64BIT, false),
    /* 0009 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_128BIT, false),
    // please write more test case.
};

INSTANTIATE_TEST_CASE_P(Test_WriteBody_WithParameter, TRegGPROT2,
                        ::testing::ValuesIn(GetSet_ParametersTable2));


/*!
 * @brief Check GEN value.
 */

TEST_F(TRegGPROT2, Check_GEN_val) {
    RegData gen_val = 0x1U;
    m_treg_gprot->SetGEN(gen_val);
    EXPECT_EQ(gen_val, m_treg_gprot->GetGEN());
    gen_val = 0x0U;
    m_treg_gprot->SetGEN(gen_val);
    EXPECT_EQ(gen_val, m_treg_gprot->GetGEN());
}


/*!
 * @brief Check DBG value.
 */

TEST_F(TRegGPROT2, Check_DBG_val) {
    RegData dbg_val = 0x1U;
    m_treg_gprot->SetDBG(dbg_val);
    EXPECT_EQ(dbg_val, m_treg_gprot->GetDBG());
    dbg_val = 0x0U;
    m_treg_gprot->SetDBG(dbg_val);
    EXPECT_EQ(dbg_val, m_treg_gprot->GetDBG());
}


/*!
 * @brief Check UME value.
 */

TEST_F(TRegGPROT2, Check_UME_val) {
    RegData ume_val = 0x1U;
    m_treg_gprot->SetUME(ume_val);
    EXPECT_EQ(ume_val, m_treg_gprot->GetUME());
    ume_val = 0x0U;
    m_treg_gprot->SetUME(ume_val);
    EXPECT_EQ(ume_val, m_treg_gprot->GetUME());
}


/*!
 * @brief Check WG value.
 */

TEST_F(TRegGPROT2, Check_WG_val) {
    RegData wg_val = 0x1U;
    m_treg_gprot->SetWG(wg_val);
    EXPECT_EQ(wg_val, m_treg_gprot->GetWG());
    wg_val = 0x0U;
    m_treg_gprot->SetWG(wg_val);
    EXPECT_EQ(wg_val, m_treg_gprot->GetWG());
}


/*!
 * @brief Check RG value.
 */

TEST_F(TRegGPROT2, Check_RG_val) {
    RegData rg_val = 0x1U;
    m_treg_gprot->SetRG(rg_val);
    EXPECT_EQ(rg_val, m_treg_gprot->GetRG());
    rg_val = 0x0U;
    m_treg_gprot->SetRG(rg_val);
    EXPECT_EQ(rg_val, m_treg_gprot->GetRG());
}

// ----------------------------------------
class TRegGPROT3 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t, bool>> {
protected:
    TRegGPROT3() {}
    virtual ~TRegGPROT3() {}
    virtual void SetUp() {
        m_treg_gprot   = new GPROT (reg_size_gprot, min_access_size_gprot, m_reg_addr, m_channel, "GPROT", CSG_MODULE, CORE_G4MH, CORE_VER_20, &m_guard_internal);
    }
    virtual void TearDown() {
        delete m_treg_gprot;
    }

public:
    MockGuardInternal m_guard_internal;
    GPROT   *m_treg_gprot;
    MockSideBand sideband;

public:
    uint32_t reg_size_gprot = SIZE_32BIT;
    uint32_t min_access_size_gprot = SIZE_8BIT;
    PhysAddr m_reg_addr = 0xFFFFFFFF;
    uint32_t m_channel = 0x0;
};


/*!
 * @brief Check WriteBody method.
 */

TEST_P(TRegGPROT3, Check_WriteBody) {
    {
        SideBand cur_sideband = sideband;
        PhysAddr cur_reg_addr = std::get<0>(GetParam());
        uint8_t *data     = std::get<1>(GetParam());
        ByteSize size     = std::get<2>(GetParam());
        bool is_write     = std::get<3>(GetParam());

        if ((size != SIZE_32BIT) && (size != SIZE_16BIT) && (size != SIZE_8BIT)) {
            ASSERT_THROW(m_treg_gprot->WriteBody(cur_reg_addr, data, size), std::range_error);
        } else {
            EXPECT_EQ(SYSERR_NONE, m_treg_gprot->WriteBody(cur_reg_addr, data, size));
            // GPROT has access size: 8bit, 16bit, 32bit.
            if (is_write) {
                if ((size == SIZE_32BIT) || (size == SIZE_16BIT)) {
                    EXPECT_EQ(0x1U, m_treg_gprot->GetGEN());
                    EXPECT_EQ(0x1U, m_treg_gprot->GetDBG());
                    EXPECT_EQ(0x1U, m_treg_gprot->GetUME());
                    EXPECT_EQ(0x1U, m_treg_gprot->GetWG());
                    EXPECT_EQ(0x1U, m_treg_gprot->GetRG());
                } else {
                    uint32_t addr_check = cur_reg_addr & 0x3;
                    if (addr_check == 0x0) {
                        EXPECT_EQ(0x1U, m_treg_gprot->GetDBG());
                        EXPECT_EQ(0x1U, m_treg_gprot->GetUME());
                        EXPECT_EQ(0x1U, m_treg_gprot->GetWG());
                        EXPECT_EQ(0x1U, m_treg_gprot->GetRG());
                    } else { // addr_check = 0x1.
                        EXPECT_EQ(0x1U, m_treg_gprot->GetGEN());
                    }
                }
            } else {
                EXPECT_EQ(0x0U, m_treg_gprot->GetGEN());
                EXPECT_EQ(0x0U, m_treg_gprot->GetDBG());
                EXPECT_EQ(0x0U, m_treg_gprot->GetUME());
                EXPECT_EQ(0x0U, m_treg_gprot->GetWG());
                EXPECT_EQ(0x0U, m_treg_gprot->GetRG());
            }
        }
    }
}

std::tuple<PhysAddr, uint8_t *, uint32_t, bool> const GetSet_ParametersTable3[] = {
    // INPUT_DATA of GPROT(PhysAddr, uint8_t *, ByteSize=)
    /* 0000 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0x53, 0x01, 0x00, 0x00}, SIZE_32BIT, true),
    /* 0001 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT, true),
    /* 0002 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT, true),
    /* 0003 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT, false),
    /* 0004 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, true),
    /* 0005 */ std::make_tuple(0x0000cafd, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, true),
    /* 0006 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, false),
    /* 0007 */ std::make_tuple(0x0000caff, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, false),
    /* 0008 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_64BIT, false),
    /* 0009 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_128BIT, false),
    // please write more test case.
};

INSTANTIATE_TEST_CASE_P(Test_WriteBody_WithParameter, TRegGPROT3,
                        ::testing::ValuesIn(GetSet_ParametersTable3));


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
