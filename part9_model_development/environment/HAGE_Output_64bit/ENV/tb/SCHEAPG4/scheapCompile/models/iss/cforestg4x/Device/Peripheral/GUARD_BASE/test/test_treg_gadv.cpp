/**
 * @file test_treg_gadv.cpp
 *
 * @date Created on: 2016
 * @author Author: Quang Lam <quang.lam.px@rvc.renesas.com>
 * @brief UNIT TEST for GADV. (SAMPLE)
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

class GADV;

// ----------------------------------------
class TRegGADV1 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t>> {
protected:
    TRegGADV1() {}
    virtual ~TRegGADV1() {}
    virtual void SetUp() {
        m_peg_gadv   = new GADV (reg_size_pegadv, min_access_size_pegadv, m_reg_addr, m_channel, "PEGADV", 0x0, PEG_MODULE, CORE_G4MH, CORE_VER_11, &m_guard_internal);
        m_crg_gadv   = new GADV (reg_size_crgadv, min_access_size_crgadv, m_reg_addr, m_channel, "CRGADV", 0x0, CRG_MODULE, CORE_G4MH, CORE_VER_11, &m_guard_internal);
    }
    virtual void TearDown() {
        delete m_peg_gadv;
        delete m_crg_gadv;
    }

public:
    MockGuardInternal m_guard_internal;
    GADV   *m_peg_gadv;
    GADV   *m_crg_gadv;
    MockSideBand sideband;

public:
    uint32_t reg_size_pegadv = SIZE_32BIT;
    uint32_t min_access_size_pegadv = SIZE_32BIT;
    uint32_t reg_size_crgadv = SIZE_32BIT;
    uint32_t min_access_size_crgadv = SIZE_8BIT;
    PhysAddr m_reg_addr = 0xFFFFFFFF;
    uint32_t m_channel = 0x0;
    uint32_t is_peg_module = 0x0;
};


/*!
 * @brief Check Initial method.
 */

TEST_F(TRegGADV1, Check_Initial_Setup) {
    ASSERT_STREQ("PEGADV", m_peg_gadv->GetName());
    ASSERT_STREQ("CRGADV", m_crg_gadv->GetName());
    uint8_t data[4] = {0x01, 0x01, 0x01, 0x01};
    uint32_t peg_init_val = 0x00000000U;
    uint32_t crg_init_val = 0x00000000U;
    m_peg_gadv->ReadBody(m_reg_addr, data, SIZE_32BIT);
    uint32_t *wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(peg_init_val, *wdata);

    m_crg_gadv->ReadBody(m_reg_addr, data, SIZE_32BIT);
    wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(crg_init_val, *wdata);
}


/*!
 * @brief Check Reset method.
 */
TEST_F(TRegGADV1, Check_Reset) {
    m_peg_gadv->Reset();
    uint8_t data[4] = {0x01, 0x01, 0x01, 0x01};
    uint32_t peg_init_val = 0x00000000U;
    uint32_t crg_init_val = 0x00000000U;
    m_peg_gadv->ReadBody(m_reg_addr, data, SIZE_32BIT);
    uint32_t *wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(peg_init_val, *wdata);

    m_crg_gadv->ReadBody(m_reg_addr, data, SIZE_32BIT);
    wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(crg_init_val, *wdata);
}


/*!
 * @brief Check WritePEG method.
 */

TEST_P(TRegGADV1, Check_WritePEG) {
    SideBand cur_sideband = sideband;
    PhysAddr cur_reg_addr = std::get<0>(GetParam());
    uint8_t *data     = std::get<1>(GetParam());
    ByteSize size     = std::get<2>(GetParam());

    EXPECT_CALL(m_guard_internal, IsWriteEn())
        .Times(AtLeast(1));
    m_guard_internal.IsWriteEn();

    // Access size for PEGADV is only 32bit.
    if (size == SIZE_32BIT) {
        EXPECT_EQ(SYSERR_NONE, m_peg_gadv->Write(cur_sideband, cur_reg_addr, data, size));
        EXPECT_EQ(0x03FFF000U, m_peg_gadv->GetADV());
    } else {
        ASSERT_THROW(m_peg_gadv->Write(cur_sideband, cur_reg_addr, data, size), std::range_error);
    }
}


/*!
 * @brief Check WriteCRG method.
 */

TEST_P(TRegGADV1, Check_WriteCRG) {
    SideBand cur_sideband = sideband;
    PhysAddr cur_reg_addr = std::get<0>(GetParam());
    uint8_t *data     = std::get<1>(GetParam());
    ByteSize size     = std::get<2>(GetParam());

    EXPECT_CALL(m_guard_internal, IsWriteEn())
        .Times(AtLeast(1));
    m_guard_internal.IsWriteEn();

    // Access size for CRGADV is 8bit, 16bit and 32bit.
    if ((size == SIZE_8BIT) || (size == SIZE_16BIT) || (size == SIZE_32BIT)) {
        EXPECT_EQ(SYSERR_NONE, m_crg_gadv->Write(cur_sideband, cur_reg_addr, data, size));
        switch (size) {
        case SIZE_16BIT:
            EXPECT_EQ(0x0000FF00U, m_crg_gadv->GetADV());
            break;
        case SIZE_8BIT:
            EXPECT_EQ(0x00000000U, m_crg_gadv->GetADV());
            break;
        case SIZE_32BIT:
        default:
            EXPECT_EQ(0x0007FF00U, m_crg_gadv->GetADV());
            break;
        }
    } else {
        ASSERT_THROW(m_crg_gadv->Write(cur_sideband, cur_reg_addr, data, size), std::range_error);
    }
}

std::tuple<PhysAddr, uint8_t *, uint32_t> const GetSet_ParametersTable[] = {
    // INPUT_DATA of GKCADV(SideBand, PhysAddr, uint8_t *, ByteSize=)
    /* 0000 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT),
    /* 0001 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT),
    /* 0002 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT),
    /* 0003 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_48BIT),
    /* 0004 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_64BIT),
    /* 0005 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_128BIT),
    // please write more test case.
};

INSTANTIATE_TEST_CASE_P(Test_Write_WithParameter, TRegGADV1,
                        ::testing::ValuesIn(GetSet_ParametersTable));


// ----------------------------------------
class TRegGADV2 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t, bool>> {
protected:
    TRegGADV2() {}
    virtual ~TRegGADV2() {}
    virtual void SetUp() {
        m_peg_gadv   = new GADV (reg_size_pegadv, min_access_size_pegadv, m_reg_addr, m_channel, "PEGADV", 0x0, PEG_MODULE, CORE_G4MH, CORE_VER_11, &m_guard_internal);
        m_crg_gadv   = new GADV (reg_size_crgadv, min_access_size_crgadv, m_reg_addr, m_channel, "CRGADV", 0x0, CRG_MODULE, CORE_G4MH, CORE_VER_11, &m_guard_internal);
    }
    virtual void TearDown() {
        delete m_peg_gadv;
        delete m_crg_gadv;
    }

public:
    MockGuardInternal m_guard_internal;
    GADV   *m_peg_gadv;
    GADV   *m_crg_gadv;
    MockSideBand sideband;

public:
    uint32_t reg_size_pegadv = SIZE_32BIT;
    uint32_t min_access_size_pegadv = SIZE_32BIT;
    uint32_t reg_size_crgadv = SIZE_32BIT;
    uint32_t min_access_size_crgadv = SIZE_8BIT;
    PhysAddr m_reg_addr = 0xFFFFFFFF;
    uint32_t m_channel = 0x0;
    uint32_t is_peg_module = 0x0;
};


/*!
 * @brief Check available write bit for WriteBody in PEG.
 */

TEST_F(TRegGADV2, Check_EnWritePEG_val) {
    // Write all available bit to 1.
    EXPECT_EQ(SYSERR_NONE, m_peg_gadv->WriteBody(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT));
    EXPECT_EQ(0x03FFF000U, m_peg_gadv->GetADV());

    // Write all available bit to 0.
    EXPECT_EQ(SYSERR_NONE, m_peg_gadv->WriteBody(0x0000cafe, new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT));
    EXPECT_EQ(0x00000000U, m_peg_gadv->GetADV());

    // Write all available bit to 1.
    EXPECT_EQ(SYSERR_NONE, m_peg_gadv->WriteBody(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT));
    EXPECT_EQ(0x03FFF000U, m_peg_gadv->GetADV());
}


/*!
 * @brief Check available write bit for WriteBody in CRG.
 */

TEST_F(TRegGADV2, Check_EnWriteCRG_val) {
    // Write all available bit to 1.
    EXPECT_EQ(SYSERR_NONE, m_crg_gadv->WriteBody(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT));
    EXPECT_EQ(0x0007FF00U, m_crg_gadv->GetADV());

    // Write all available bit to 0.
    EXPECT_EQ(SYSERR_NONE, m_crg_gadv->WriteBody(0x0000cafe, new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT));
    EXPECT_EQ(0x00000000U, m_crg_gadv->GetADV());

    // Write all available bit to 1.
    EXPECT_EQ(SYSERR_NONE, m_crg_gadv->WriteBody(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT));
    EXPECT_EQ(0x0007FF00U, m_crg_gadv->GetADV());
}

/*!
 * @brief Check WriteBody method.
 */

TEST_P(TRegGADV2, Check_WriteBodyPEG) {
    {
        SideBand cur_sideband = sideband;
        PhysAddr cur_reg_addr = std::get<0>(GetParam());
        uint8_t *data     = std::get<1>(GetParam());
        ByteSize size     = std::get<2>(GetParam());
        bool is_write     = std::get<3>(GetParam());

        if (size != SIZE_32BIT) {
            ASSERT_THROW(m_peg_gadv->WriteBody(cur_reg_addr, data, size), std::range_error);
        } else {
            EXPECT_EQ(SYSERR_NONE, m_peg_gadv->WriteBody(cur_reg_addr, data, size));
            if (is_write) {
                if (size == SIZE_32BIT) {
                    EXPECT_EQ(0x03FFF000U, m_peg_gadv->GetADV());
                }
            } else {
                EXPECT_EQ(0x00000000U, m_peg_gadv->GetADV());
            }
        }
    }
}

std::tuple<PhysAddr, uint8_t *, uint32_t, bool> const GetSet_ParametersTablePEG[] = {
    // INPUT_DATA of GKCADV(PhysAddr, uint8_t *, ByteSize=)
    /* 0000 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT, true),
    /* 0001 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT, false),
    /* 0002 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT, false),
    /* 0003 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, false),
    /* 0004 */ std::make_tuple(0x0000cafd, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, false),
    /* 0005 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, false),
    /* 0006 */ std::make_tuple(0x0000caff, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, false),
    /* 0007 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_64BIT, false),
    /* 0008 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_128BIT, false),
    // please write more test case.
};

INSTANTIATE_TEST_CASE_P(Test_WriteBody_WithParameterPEG, TRegGADV2,
                        ::testing::ValuesIn(GetSet_ParametersTablePEG));


// Test CRGADV with another class.

// ----------------------------------------
class TRegGADV3 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t, bool>> {
protected:
    TRegGADV3() {}
    virtual ~TRegGADV3() {}
    virtual void SetUp() {
        m_peg_gadv   = new GADV (reg_size_pegadv, min_access_size_pegadv, m_reg_addr, m_channel, "PEGADV", 0x0, PEG_MODULE, CORE_G4MH, CORE_VER_11, &m_guard_internal);
        m_crg_gadv   = new GADV (reg_size_crgadv, min_access_size_crgadv, m_reg_addr, m_channel, "CRGADV", 0x0, CRG_MODULE, CORE_G4MH, CORE_VER_11, &m_guard_internal);
    }
    virtual void TearDown() {
        delete m_peg_gadv;
        delete m_crg_gadv;
    }

public:
    MockGuardInternal m_guard_internal;
    GADV   *m_peg_gadv;
    GADV   *m_crg_gadv;
    MockSideBand sideband;

public:
    uint32_t reg_size_pegadv = SIZE_32BIT;
    uint32_t min_access_size_pegadv = SIZE_32BIT;
    uint32_t reg_size_crgadv = SIZE_32BIT;
    uint32_t min_access_size_crgadv = SIZE_8BIT;
    PhysAddr m_reg_addr = 0xFFFFFFFF;
    uint32_t m_channel = 0x0;
    uint32_t is_peg_module = 0x0;
};


TEST_P(TRegGADV3, Check_WriteBodyCRG) {
    {
        SideBand cur_sideband = sideband;
        PhysAddr cur_reg_addr = std::get<0>(GetParam());
        uint8_t *data     = std::get<1>(GetParam());
        ByteSize size     = std::get<2>(GetParam());
        bool is_write     = std::get<3>(GetParam());

        if ((size != SIZE_32BIT) && (size != SIZE_16BIT) && (size != SIZE_8BIT)) {
            ASSERT_THROW(m_crg_gadv->WriteBody(cur_reg_addr, data, size), std::range_error);
        } else {
            EXPECT_EQ(SYSERR_NONE, m_crg_gadv->WriteBody(cur_reg_addr, data, size));
            if (is_write) {
                if (size == SIZE_32BIT) {
                    EXPECT_EQ(0x0007FF00U, m_crg_gadv->GetADV());
                } else if (size == SIZE_16BIT) {
                    uint32_t addr_check = cur_reg_addr & 0x3;
                    if (addr_check == 0x0) {
                        EXPECT_EQ(0x0000FF00U, m_crg_gadv->GetADV());
                    } else { // addr_check = 0x1.
                        EXPECT_EQ(0x00070000U, m_crg_gadv->GetADV());
                    }
                } else if (size == SIZE_8BIT) {
                    uint32_t addr_check = cur_reg_addr & 0x3;
                    switch (addr_check) {
                    case 0x0:
                        EXPECT_EQ(0x00000000U, m_crg_gadv->GetADV());
                        break;
                    case 0x1:
                        EXPECT_EQ(0x0000FF00U, m_crg_gadv->GetADV());
                        break;
                    case 0x2:
                        EXPECT_EQ(0x00070000U, m_crg_gadv->GetADV());
                        break;
                    case 0x3:
                    default:
                        EXPECT_EQ(0x00000000U, m_crg_gadv->GetADV());
                        break;
                    }
                }
            } else {
                EXPECT_EQ(0x00000000U, m_crg_gadv->GetADV());
            }
        }
    }
}

std::tuple<PhysAddr, uint8_t *, uint32_t, bool> const GetSet_ParametersTableCRG[] = {
    // INPUT_DATA of GKCADV(PhysAddr, uint8_t *, ByteSize=)
    /* 0000 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT, true),
    /* 0001 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT, true),
    /* 0002 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT, true),
    /* 0003 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, true),
    /* 0004 */ std::make_tuple(0x0000cafd, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, true),
    /* 0005 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, true),
    /* 0006 */ std::make_tuple(0x0000caff, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, true),
    /* 0007 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_64BIT, false),
    /* 0008 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_128BIT, false),
    // please write more test case.
};

INSTANTIATE_TEST_CASE_P(Test_WriteBody_WithParameterCRG, TRegGADV3,
                        ::testing::ValuesIn(GetSet_ParametersTableCRG));


/*!
 * @brief Check ADV value.
 */

TEST_F(TRegGADV3, Check_PEGADV_val) {
    RegData gen_val = 0xCAFECAFEU;
    m_peg_gadv->SetADV(gen_val);
    EXPECT_EQ(gen_val, m_peg_gadv->GetADV());
    gen_val = 0xA5A5A5A5;
    m_peg_gadv->SetADV(gen_val);
    EXPECT_EQ(gen_val, m_peg_gadv->GetADV());
}

TEST_F(TRegGADV3, Check_CRGADV_val) {
    RegData gen_val = 0xCAFECAFEU;
    m_crg_gadv->SetADV(gen_val);
    EXPECT_EQ(gen_val, m_crg_gadv->GetADV());
    gen_val = 0xA5A5A5A5;
    m_crg_gadv->SetADV(gen_val);
    EXPECT_EQ(gen_val, m_crg_gadv->GetADV());
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
