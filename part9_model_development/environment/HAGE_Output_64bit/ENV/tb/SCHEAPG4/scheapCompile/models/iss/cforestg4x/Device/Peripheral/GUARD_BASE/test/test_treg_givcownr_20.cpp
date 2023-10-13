/**
 * @file test_treg_givcownr_20.cpp
 *
 * @date Created on: 2016
 * @author Author: Hoanh Ho <hoanh.ho.px@renesas.com>
 * @brief UNIT TEST for GIVCOWNR. (SAMPLE)
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
 using ::testing::Return;

class GIVCOWNR;

// ----------------------------------------
class TRegGIVCOWNR1 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t>> {
protected:
    TRegGIVCOWNR1() {}
    virtual ~TRegGIVCOWNR1() {}
    virtual void SetUp() {
        m_treg_givcownr   = new GIVCOWNR (reg_size_gspid, min_access_size_gspid, m_reg_addr, m_channel, "GIVCOWNR", &m_guard_internal);
    }
    virtual void TearDown() {
        delete m_treg_givcownr;
    }

public:
    MockGuardInternal m_guard_internal;
    GIVCOWNR   *m_treg_givcownr;
    MockSideBand sideband;

public:
    uint32_t reg_size_gspid = SIZE_32BIT;
    uint32_t min_access_size_gspid = SIZE_8BIT;
    PhysAddr m_reg_addr = 0xFFFFFFFF;
    uint32_t m_channel = 0x0;
};


/*!
 * @brief Check Initial method.
 */

TEST_F(TRegGIVCOWNR1, Check_Initial_Setup) {
    ASSERT_STREQ("GIVCOWNR", m_treg_givcownr->GetName());
    uint8_t data[4] = {0x01, 0x01, 0x01, 0x01};
    uint32_t init_val = 0x00000000U;
    m_treg_givcownr->ReadBody(m_reg_addr, data, SIZE_32BIT);
    uint32_t *wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(init_val, *wdata);
    m_treg_givcownr->ReadBody(m_reg_addr, data, SIZE_16BIT);
    uint32_t *wdata1 = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(init_val, *wdata1);
    m_treg_givcownr->ReadBody(m_reg_addr, data, SIZE_8BIT);
    uint32_t *wdata2 = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(init_val, *wdata2);
}


/*!
 * @brief Check Reset method.
 */
TEST_F(TRegGIVCOWNR1, Check_Reset) {
    m_treg_givcownr->Reset();
    uint8_t data[4] = {0x01, 0x01, 0x01, 0x01};
    uint32_t init_val = 0x00000000U;
    m_treg_givcownr->ReadBody(m_reg_addr, data, SIZE_32BIT);
    uint32_t *wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(init_val, *wdata);
}


/*!
 * @brief Check Write method.
 */

TEST_P(TRegGIVCOWNR1, Check_Write) {
    SideBand cur_sideband = sideband;
    PhysAddr cur_reg_addr = std::get<0>(GetParam());
    uint8_t *data     = std::get<1>(GetParam());
    ByteSize size     = std::get<2>(GetParam());

    EXPECT_CALL(m_guard_internal, IsWriteEn())
        .Times(AtLeast(1));
    m_guard_internal.IsWriteEn();
    if ((size == SIZE_8BIT) || (size == SIZE_16BIT) || (size == SIZE_32BIT)) {
        EXPECT_EQ(SYSERR_NONE, m_treg_givcownr->Write(cur_sideband, cur_reg_addr, data, size));
        switch (size) {
        case SIZE_16BIT:
            EXPECT_EQ(0x00000000U, m_treg_givcownr->GetSPID());
            break;
        case SIZE_8BIT:
            EXPECT_EQ(0x00000000U, m_treg_givcownr->GetSPID());
            break;
        case SIZE_32BIT:
        default:
            EXPECT_EQ(0x00000000U, m_treg_givcownr->GetSPID());
            break;
        }
    } else {
        ASSERT_THROW(m_treg_givcownr->Write(cur_sideband, cur_reg_addr, data, size), std::range_error);
    }
}

std::tuple<PhysAddr, uint8_t *, uint32_t> const GetSet_ParametersTable[] = {
    // INPUT_DATA of GIVCOWNR(SideBand, PhysAddr, uint8_t *, ByteSize=)
    /* 0000 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT),
    /* 0001 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT),
    /* 0002 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT),
    /* 0003 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_48BIT),
    /* 0004 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_64BIT),
    /* 0005 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_128BIT),
    // please write more test case.
};

INSTANTIATE_TEST_CASE_P(Test_Write_WithParameter, TRegGIVCOWNR1,
                        ::testing::ValuesIn(GetSet_ParametersTable));


// ----------------------------------------
class TRegGIVCOWNR2 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t, bool>> {
protected:
    TRegGIVCOWNR2() {}
    virtual ~TRegGIVCOWNR2() {}
    virtual void SetUp() {
        m_treg_givcownr   = new GIVCOWNR (reg_size_gspid, min_access_size_gspid, m_reg_addr, m_channel, "GIVCOWNR", &m_guard_internal);
    }
    virtual void TearDown() {
        delete m_treg_givcownr;
    }

public:
    MockGuardInternal m_guard_internal;
    GIVCOWNR   *m_treg_givcownr;
    MockSideBand sideband;

public:
    uint32_t reg_size_gspid = SIZE_32BIT;
    uint32_t min_access_size_gspid = SIZE_8BIT;
    PhysAddr m_reg_addr = 0xFFFFFFFF;
    uint32_t m_channel = 0x0;
};


/*!
 * @brief Check available write bit..
 */

TEST_F(TRegGIVCOWNR2, Check_EnWrite_val) {
    // Write all available bit to 1.
    EXPECT_EQ(SYSERR_NONE, m_treg_givcownr->WriteBody(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT));
    EXPECT_EQ(0x00000000U, m_treg_givcownr->GetSPID());

    // Write all available bit to 0.
    EXPECT_EQ(SYSERR_NONE, m_treg_givcownr->WriteBody(0x0000cafe, new uint8_t[4] {0x00, 0x00, 0x00, 0x00}, SIZE_32BIT));
    EXPECT_EQ(0x00000000U, m_treg_givcownr->GetSPID());

    // Write all available bit to 1.
    EXPECT_EQ(SYSERR_NONE, m_treg_givcownr->WriteBody(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT));
    EXPECT_EQ(0x00000000U, m_treg_givcownr->GetSPID());
}


/*!
 * @brief Check WriteBody method.
 */

TEST_P(TRegGIVCOWNR2, Check_WriteBody) {
    {
        SideBand cur_sideband = sideband;
        PhysAddr cur_reg_addr = std::get<0>(GetParam());
        uint8_t *data     = std::get<1>(GetParam());
        ByteSize size     = std::get<2>(GetParam());
        bool is_write     = std::get<3>(GetParam());

        if ((size != SIZE_32BIT) && (size != SIZE_16BIT) && (size != SIZE_8BIT)) {
            ASSERT_THROW(m_treg_givcownr->WriteBody(cur_reg_addr, data, size), std::range_error);
        } else {
            EXPECT_EQ(SYSERR_NONE, m_treg_givcownr->WriteBody(cur_reg_addr, data, size));
            if (is_write) {
                if (size == SIZE_32BIT) {
                    EXPECT_EQ(0x00000000U, m_treg_givcownr->GetSPID());
                } else if (size == SIZE_16BIT) {
                    // Get 2 lowest bit in address.
                    uint32_t addr_check = cur_reg_addr & 0x3;
                    if (addr_check == 0x0) {
                        EXPECT_EQ(0x00000000U, m_treg_givcownr->GetSPID());
                    } else { // addr_check = 0x2.
                        EXPECT_EQ(0x00000000U, m_treg_givcownr->GetSPID());
                    }
                } else if (size == SIZE_8BIT) {
                    // Get 2 lowest bit in address.
                    uint32_t addr_check = cur_reg_addr & 0x3;
                    switch (addr_check) {
                    case 0x0:
                        EXPECT_EQ(0x00000000U, m_treg_givcownr->GetSPID());
                        break;
                    case 0x1:
                        EXPECT_EQ(0x00000000U, m_treg_givcownr->GetSPID());
                        break;
                    case 0x2:
                        EXPECT_EQ(0x00000000U, m_treg_givcownr->GetSPID());
                        break;
                    case 0x3:
                    default:
                        EXPECT_EQ(0x00000000U, m_treg_givcownr->GetSPID());
                        break;
                    }
                }
            } else {
                EXPECT_EQ(0x00000000U, m_treg_givcownr->GetSPID());
            }
        }
    }
}

std::tuple<PhysAddr, uint8_t *, uint32_t, bool> const GetSet_ParametersTable2[] = {
    // INPUT_DATA of GIVCOWNR(PhysAddr, uint8_t *, ByteSize=)
    /* 0000 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT, true),
    /* 0001 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT, true),
    /* 0002 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT, true),
    /* 0003 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, true),
    /* 0004 */ std::make_tuple(0x0000cafd, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, true),
    /* 0005 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, true),
    /* 0006 */ std::make_tuple(0x0000caff, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT, true),
//    /* 0007 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_64BIT, false),
//    /* 0008 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_128BIT, false),
    // please write more test case.
};

INSTANTIATE_TEST_CASE_P(Test_WriteBody_WithParameter, TRegGIVCOWNR2,
                        ::testing::ValuesIn(GetSet_ParametersTable2));


/*!
 * @brief Check SPID value.
 */

//TEST_F(TRegGIVCOWNR2, Check_SPID_val) {
//    uint32_t chn = 0x0;
//    RegData gen_val = 0xCAFECAFEU;
////    m_guard_internal->SetCrgSpidOwner(chn, gen_val);
//    EXPECT_CALL(m_guard_internal, SetCrgSpidOwner(chn)).WillOnce(Return(gen_val));
//    EXPECT_EQ(gen_val, m_treg_givcownr->GetSPID());
//    gen_val = 0xA5A5A5A5U;
////    m_treg_givcownr->SetCrgSpidOwner(chn, gen_val);
//    EXPECT_EQ(gen_val, m_treg_givcownr->GetSPID());
//}


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
