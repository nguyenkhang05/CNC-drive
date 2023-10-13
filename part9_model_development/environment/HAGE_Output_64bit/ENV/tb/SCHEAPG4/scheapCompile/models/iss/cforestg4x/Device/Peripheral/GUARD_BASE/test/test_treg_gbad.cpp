/**
 * @file test_treg_gbad.cpp
 *
 * @date Created on: 2016
 * @author Author: Quang Lam <quang.lam.px@rvc.renesas.com>
 * @brief UNIT TEST for GBAD. (SAMPLE)
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

class GBAD;

// ----------------------------------------
class TRegGBAD1 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t>> {
protected:
    TRegGBAD1() {}
    virtual ~TRegGBAD1() {}
    virtual void SetUp() {
        m_peg_gbad   = new GBAD (reg_size_pegbad, min_access_size_pegbad, m_reg_addr, m_channel, "PEGBAD", 21, PEG_MODULE, CORE_G4MH, CORE_VER_11, &m_guard_internal);
        m_crg_gbad   = new GBAD (reg_size_crgbad, min_access_size_crgbad, m_reg_addr, m_channel, "CRGBAD", 21, CRG_MODULE, CORE_G4MH, CORE_VER_11, &m_guard_internal);
    }
    virtual void TearDown() {
        delete m_peg_gbad;
        delete m_crg_gbad;
    }

public:
    MockGuardInternal m_guard_internal;
    GBAD   *m_peg_gbad;
    GBAD   *m_crg_gbad;
    MockSideBand sideband;

public:
    uint32_t reg_size_pegbad = SIZE_32BIT;
    uint32_t min_access_size_pegbad = SIZE_32BIT;
    uint32_t reg_size_crgbad = SIZE_32BIT;
    uint32_t min_access_size_crgbad = SIZE_8BIT;
    PhysAddr m_reg_addr = 0xFFFFFFFF;
    uint32_t m_channel = 0x0;
    uint32_t is_peg_module = 0x0;
};


/*!
 * @brief Check Initial method.
 */

TEST_F(TRegGBAD1, Check_Initial_Setup) {
    ASSERT_STREQ("PEGBAD", m_peg_gbad->GetName());
    ASSERT_STREQ("CRGBAD", m_crg_gbad->GetName());
    uint8_t data[4] = {0x01, 0x01, 0x01, 0x01};
    uint32_t peg_init_val = 0xFC000000U;
    uint32_t crg_init_val = 0x00000000U;
    m_peg_gbad->ReadBody(m_reg_addr, data, SIZE_32BIT);
    uint32_t *wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(peg_init_val, *wdata);

    m_crg_gbad->ReadBody(m_reg_addr, data, SIZE_32BIT);
    wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(crg_init_val, *wdata);
}


/*!
 * @brief Check Reset method.
 */
TEST_F(TRegGBAD1, Check_Reset) {
    m_peg_gbad->Reset();
    uint8_t data[4] = {0x01, 0x01, 0x01, 0x01};
    uint32_t peg_init_val = 0xFC000000U;
    uint32_t crg_init_val = 0x00000000U;
    m_peg_gbad->ReadBody(m_reg_addr, data, SIZE_32BIT);
    uint32_t *wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(peg_init_val, *wdata);

    m_crg_gbad->ReadBody(m_reg_addr, data, SIZE_32BIT);
    wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(crg_init_val, *wdata);
}


/*!
 * @brief Check WritePEG method.
 */

TEST_P(TRegGBAD1, Check_WritePEG) {
    SideBand cur_sideband = sideband;
    PhysAddr cur_reg_addr = std::get<0>(GetParam());
    uint8_t *data     = std::get<1>(GetParam());
    ByteSize size     = std::get<2>(GetParam());

    EXPECT_CALL(m_guard_internal, IsWriteEn())
        .Times(AtLeast(1));
    m_guard_internal.IsWriteEn();

    // Access size for PEGBAD is only 32bit.
    if (size == SIZE_32BIT) {
        EXPECT_EQ(SYSERR_NONE, m_peg_gbad->Write(cur_sideband, cur_reg_addr, data, size));
        EXPECT_EQ(0xFFFFF000U, m_peg_gbad->GetBAD());
    } else {
        ASSERT_THROW(m_peg_gbad->Write(cur_sideband, cur_reg_addr, data, size), std::range_error);
    }
}


/*!
 * @brief Check WriteCRG method.
 */

TEST_P(TRegGBAD1, Check_WriteCRG) {
    SideBand cur_sideband = sideband;
    PhysAddr cur_reg_addr = std::get<0>(GetParam());
    uint8_t *data     = std::get<1>(GetParam());
    ByteSize size     = std::get<2>(GetParam());

    EXPECT_CALL(m_guard_internal, IsWriteEn())
        .Times(AtLeast(1));
    m_guard_internal.IsWriteEn();

    // Access size for CRGBAD is 8bit, 16bit and 32bit.
    if ((size == SIZE_8BIT) || (size == SIZE_16BIT) || (size == SIZE_32BIT)) {
        EXPECT_EQ(SYSERR_NONE, m_crg_gbad->Write(cur_sideband, cur_reg_addr, data, size));
        switch (size) {
        case SIZE_16BIT:
            EXPECT_EQ(0x0000FF00U, m_crg_gbad->GetBAD());
            break;
        case SIZE_8BIT:
            EXPECT_EQ(0x00000000U, m_crg_gbad->GetBAD());
            break;
        case SIZE_32BIT:
        default:
            EXPECT_EQ(0x0007FF00U, m_crg_gbad->GetBAD());
            break;
        }
    } else {
        ASSERT_THROW(m_crg_gbad->Write(cur_sideband, cur_reg_addr, data, size), std::range_error);
    }
}

std::tuple<PhysAddr, uint8_t *, uint32_t> const GetSet_ParametersTable[] = {
    // INPUT_DATA of GBAD(SideBand, PhysAddr, uint8_t *, ByteSize=)
    /* 0000 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT),
    /* 0001 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT),
    /* 0002 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT),
    /* 0003 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_48BIT),
    /* 0004 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_64BIT),
    /* 0005 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_128BIT),
    // please write more test case.
};

INSTANTIATE_TEST_CASE_P(Test_Write_WithParameter, TRegGBAD1,
                        ::testing::ValuesIn(GetSet_ParametersTable));


// ----------------------------------------
class TRegGBAD2 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t, bool>> {
protected:
    TRegGBAD2() {}
    virtual ~TRegGBAD2() {}
    virtual void SetUp() {
        m_peg_gbad   = new GBAD (reg_size_pegbad, min_access_size_pegbad, m_reg_addr, m_channel, "PEGBAD", 0x0, PEG_MODULE, CORE_G4MH, CORE_VER_11, &m_guard_internal);
        m_crg_gbad   = new GBAD (reg_size_crgbad, min_access_size_crgbad, m_reg_addr, m_channel, "CRGBAD", 0x0, CRG_MODULE, CORE_G4MH, CORE_VER_11, &m_guard_internal);
    }
    virtual void TearDown() {
        delete m_peg_gbad;
        delete m_crg_gbad;
    }

public:
    MockGuardInternal m_guard_internal;
    GBAD   *m_peg_gbad;
    GBAD   *m_crg_gbad;
    MockSideBand sideband;

public:
    uint32_t reg_size_pegbad = SIZE_32BIT;
    uint32_t min_access_size_pegbad = SIZE_32BIT;
    uint32_t reg_size_crgbad = SIZE_32BIT;
    uint32_t min_access_size_crgbad = SIZE_8BIT;
    PhysAddr m_reg_addr = 0xFFFFFFFF;
    uint32_t m_channel = 0x0;
    uint32_t is_peg_module = 0x0;
};


/*!
 * @brief Check available write bit for WriteBody in PEG.
 */

TEST_F(TRegGBAD2, Check_EnWritePEG_val) {
    // Write all available bit to 1.
    EXPECT_EQ(SYSERR_NONE, m_peg_gbad->WriteBody(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT));
    EXPECT_EQ(0xFFFFF000U, m_peg_gbad->GetBAD());

    // Write all available bit to 0.
    EXPECT_EQ(SYSERR_NONE, m_peg_gbad->WriteBody(0x0000cafe, new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT));
    EXPECT_EQ(0xFC000000U, m_peg_gbad->GetBAD());

    // Write all available bit to 1.
    EXPECT_EQ(SYSERR_NONE, m_peg_gbad->WriteBody(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT));
    EXPECT_EQ(0xFFFFF000U, m_peg_gbad->GetBAD());
}


/*!
 * @brief Check available write bit for WriteBody in CRG.
 */

TEST_F(TRegGBAD2, Check_EnWriteCRG_val) {
    // Write all available bit to 1.
    EXPECT_EQ(SYSERR_NONE, m_crg_gbad->WriteBody(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT));
    EXPECT_EQ(0x0007FF00U, m_crg_gbad->GetBAD());

    // Write all available bit to 0.
    EXPECT_EQ(SYSERR_NONE, m_crg_gbad->WriteBody(0x0000cafe, new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT));
    EXPECT_EQ(0x00000000U, m_crg_gbad->GetBAD());

    // Write all available bit to 1.
    EXPECT_EQ(SYSERR_NONE, m_crg_gbad->WriteBody(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT));
    EXPECT_EQ(0x0007FF00U, m_crg_gbad->GetBAD());
}

/*!
 * @brief Check WriteBody method.
 */

TEST_P(TRegGBAD2, Check_WriteBodyPEG) {
    {
        SideBand cur_sideband = sideband;
        PhysAddr cur_reg_addr = std::get<0>(GetParam());
        uint8_t *data     = std::get<1>(GetParam());
        ByteSize size     = std::get<2>(GetParam());
        bool is_write     = std::get<3>(GetParam());

        if (size != SIZE_32BIT) {
            ASSERT_THROW(m_peg_gbad->WriteBody(cur_reg_addr, data, size), std::range_error);
        } else {
            EXPECT_EQ(SYSERR_NONE, m_peg_gbad->WriteBody(cur_reg_addr, data, size));
            if (is_write) {
                if (size == SIZE_32BIT) {
                    EXPECT_EQ(0xFFFFF000, m_peg_gbad->GetBAD());
                }
            } else {
                EXPECT_EQ(0xFC000000, m_peg_gbad->GetBAD());
            }
        }
    }
}

std::tuple<PhysAddr, uint8_t *, uint32_t, bool> const GetSet_ParametersTablePEG[] = {
    // INPUT_DATA of GBAD(PhysAddr, uint8_t *, ByteSize=)
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

INSTANTIATE_TEST_CASE_P(Test_WriteBodyPEG_WithParameter, TRegGBAD2,
                        ::testing::ValuesIn(GetSet_ParametersTablePEG));


// ----------------------------------------
class TRegGBAD3 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t, bool>> {
protected:
    TRegGBAD3() {}
    virtual ~TRegGBAD3() {}
    virtual void SetUp() {
        m_peg_gbad   = new GBAD (reg_size_pegbad, min_access_size_pegbad, m_reg_addr, m_channel, "PEGBAD", 0x0, PEG_MODULE, CORE_G4MH, CORE_VER_11, &m_guard_internal);
        m_crg_gbad   = new GBAD (reg_size_crgbad, min_access_size_crgbad, m_reg_addr, m_channel, "CRGBAD", 0x0, CRG_MODULE, CORE_G4MH, CORE_VER_11, &m_guard_internal);
    }
    virtual void TearDown() {
        delete m_peg_gbad;
        delete m_crg_gbad;
    }

public:
    MockGuardInternal m_guard_internal;
    GBAD   *m_peg_gbad;
    GBAD   *m_crg_gbad;
    MockSideBand sideband;

public:
    uint32_t reg_size_pegbad = SIZE_32BIT;
    uint32_t min_access_size_pegbad = SIZE_32BIT;
    uint32_t reg_size_crgbad = SIZE_32BIT;
    uint32_t min_access_size_crgbad = SIZE_8BIT;
    PhysAddr m_reg_addr = 0xFFFFFFFF;
    uint32_t m_channel = 0x0;
    uint32_t is_peg_module = 0x0;
};

TEST_P(TRegGBAD3, Check_WriteBodyCRG) {
    {
        SideBand cur_sideband = sideband;
        PhysAddr cur_reg_addr = std::get<0>(GetParam());
        uint8_t *data     = std::get<1>(GetParam());
        ByteSize size     = std::get<2>(GetParam());
        bool is_write     = std::get<3>(GetParam());

        if ((size != SIZE_32BIT) && (size != SIZE_16BIT) && (size != SIZE_8BIT)) {
            ASSERT_THROW(m_crg_gbad->WriteBody(cur_reg_addr, data, size), std::range_error);
        } else {
            EXPECT_EQ(SYSERR_NONE, m_crg_gbad->WriteBody(cur_reg_addr, data, size));
            if (is_write) {
                if (size == SIZE_32BIT) {
                    EXPECT_EQ(0x0007FF00U, m_crg_gbad->GetBAD());
                } else if (size == SIZE_16BIT) {
                    uint32_t addr_check = cur_reg_addr & 0x3;
                    if (addr_check == 0x0) {
                        EXPECT_EQ(0x0000FF00U, m_crg_gbad->GetBAD());
                    } else { // addr_check = 0x1.
                        EXPECT_EQ(0x00070000U, m_crg_gbad->GetBAD());
                    }
                } else if (size == SIZE_8BIT) {
                    uint32_t addr_check = cur_reg_addr & 0x3;
                    switch (addr_check) {
                    case 0x0:
                        EXPECT_EQ(0x00000000U, m_crg_gbad->GetBAD());
                        break;
                    case 0x1:
                        EXPECT_EQ(0x0000FF00U, m_crg_gbad->GetBAD());
                        break;
                    case 0x2:
                        EXPECT_EQ(0x00070000U, m_crg_gbad->GetBAD());
                        break;
                    case 0x3:
                    default:
                        EXPECT_EQ(0x00000000U, m_crg_gbad->GetBAD());
                        break;
                    }
                }
            } else {
                EXPECT_EQ(0x00000000U, m_crg_gbad->GetBAD());
            }
        }
    }
}

std::tuple<PhysAddr, uint8_t *, uint32_t, bool> const GetSet_ParametersTableCRG[] = {
    // INPUT_DATA of GBAD(PhysAddr, uint8_t *, ByteSize=)
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

INSTANTIATE_TEST_CASE_P(Test_WriteBodyCRG_WithParameter, TRegGBAD3,
                        ::testing::ValuesIn(GetSet_ParametersTableCRG));


/*!
 * @brief Check BAD value.
 */

TEST_F(TRegGBAD3, Check_PEGBAD_val) {
    RegData gen_val = 0xCAFECAFEU;
    m_peg_gbad->SetBAD(gen_val);
    EXPECT_EQ(gen_val, m_peg_gbad->GetBAD());
    gen_val = 0xA5A5A5A5U;
    m_peg_gbad->SetBAD(gen_val);
    EXPECT_EQ(gen_val, m_peg_gbad->GetBAD());
}

TEST_F(TRegGBAD3, Check_CRGBAD_val) {
    RegData gen_val = 0xCAFECAFEU;
    m_crg_gbad->SetBAD(gen_val);
    EXPECT_EQ(gen_val, m_crg_gbad->GetBAD());
    gen_val = 0xA5A5A5A5U;
    m_crg_gbad->SetBAD(gen_val);
    EXPECT_EQ(gen_val, m_crg_gbad->GetBAD());
}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
