/**
 * @file test_treg_gerrclr.cpp
 *
 * @date Created on: 2016
 * @author Author: Quang Lam <quang.lam.px@rvc.renesas.com>
 * @brief UNIT TEST for GERRCLR. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include <memory>
#include <map>
#include <stdexcept>
#include "treg_guard.h"
#include "mock_sideband.h"
#include "mock_gerrstat.h"
#include "gmock/gmock.h"

 using ::testing::AtLeast;
// using ::testing::_;
// using ::testing::Return;

class GERRCLR;

// ----------------------------------------
class TRegGERRCLR1 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t>> {
protected:
    TRegGERRCLR1() {}
    virtual ~TRegGERRCLR1() {}
    virtual void SetUp() {
        m_treg_gerrclr   = new GERRCLR (reg_size_gerrclr, min_access_size_gerrclr, m_reg_addr, &m_treg_gerrstat, "GERRCLR", CORE_G4MH, CORE_VER_11);
    }
    virtual void TearDown() {
        delete m_treg_gerrclr;
    }

public:
    MockGERRSTAT   m_treg_gerrstat;
    GERRCLR   *m_treg_gerrclr;
    MockSideBand sideband;

public:
    uint32_t reg_size_gerrclr = SIZE_32BIT;
    uint32_t min_access_size_gerrclr = SIZE_8BIT;
    PhysAddr m_reg_addr = 0xFFFFFFFF;
};


/*!
 * @brief Check Initial method.
 */

TEST_F(TRegGERRCLR1, Check_Initial_Setup) {
    ASSERT_STREQ("GERRCLR", m_treg_gerrclr->GetName());
    uint8_t data[4] = {0x01, 0x01, 0x01, 0x01};
    uint32_t init_val = 0x00000000U;
    m_treg_gerrclr->ReadBody(m_reg_addr, data, SIZE_32BIT);
    uint32_t *wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(init_val, *wdata);
}


/*!
 * @brief Check Reset method.
 */
TEST_F(TRegGERRCLR1, Check_Reset) {
    m_treg_gerrclr->Reset();
    uint8_t data[4] = {0x01, 0x01, 0x01, 0x01};
    uint32_t init_val = 0x00000000U;
    m_treg_gerrclr->ReadBody(m_reg_addr, data, SIZE_32BIT);
    uint32_t *wdata = reinterpret_cast<uint32_t *> (data);
    EXPECT_EQ(init_val, *wdata);
}


/*!
 * @brief Check Write method.
 */

TEST_P(TRegGERRCLR1, Check_Write) {
    SideBand cur_sideband = sideband;
    PhysAddr cur_reg_addr = std::get<0>(GetParam());
    uint8_t *data     = std::get<1>(GetParam());
    ByteSize size     = std::get<2>(GetParam());

    EXPECT_CALL(m_treg_gerrstat, SetERR(0x1))
      .Times(AtLeast(1));
    EXPECT_CALL(m_treg_gerrstat, SetOVF(0x1))
      .Times(AtLeast(1));
    if ((size != SIZE_32BIT) && (size != SIZE_16BIT) && (size != SIZE_8BIT)) {
        EXPECT_CALL(m_treg_gerrstat, SetOVF(0x0))
          .Times(0);
        EXPECT_CALL(m_treg_gerrstat, SetERR(0x0))
          .Times(0);
    } else {
        EXPECT_CALL(m_treg_gerrstat, SetOVF(0x0))
          .Times(AtLeast(1));
        EXPECT_CALL(m_treg_gerrstat, SetERR(0x0))
          .Times(AtLeast(1));
    }

    m_treg_gerrstat.SetERR(1);
    m_treg_gerrstat.SetOVF(1);

    if ((size == SIZE_8BIT) || (size == SIZE_16BIT) || (size == SIZE_32BIT)) {
        EXPECT_EQ(SYSERR_NONE, m_treg_gerrclr->Write(cur_sideband, cur_reg_addr, data, size));
    } else {
        ASSERT_THROW(m_treg_gerrclr->Write(cur_sideband, cur_reg_addr, data, size), std::range_error);
    }
}

std::tuple<PhysAddr, uint8_t *, uint32_t> const GetSet_ParametersTable[] = {
    // INPUT_DATA of GERRCLR(SideBand, PhysAddr, uint8_t *, ByteSize=)
    /* 0000 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT),
    /* 0001 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT),
    /* 0002 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT),
    /* 0003 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_48BIT),
    /* 0004 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_64BIT),
    /* 0005 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_128BIT),
    // please write more test case.
};

INSTANTIATE_TEST_CASE_P(Test_Write_WithParameter, TRegGERRCLR1,
                        ::testing::ValuesIn(GetSet_ParametersTable));


// ----------------------------------------
class TRegGERRCLR2 : public testing::Test,
                     public testing::WithParamInterface<std::tuple<PhysAddr, uint8_t *, uint32_t>> {
protected:
    TRegGERRCLR2() {}
    virtual ~TRegGERRCLR2() {}
    virtual void SetUp() {
        m_treg_gerrclr   = new GERRCLR (reg_size_gerrclr, min_access_size_gerrclr, m_reg_addr, &m_treg_gerrstat, "GERRCLR", CORE_G4MH, CORE_VER_11);
    }
    virtual void TearDown() {
        delete m_treg_gerrclr;
    }

public:
    MockGERRSTAT   m_treg_gerrstat;
    GERRCLR   *m_treg_gerrclr;
    MockSideBand sideband;

public:
    uint32_t reg_size_gerrclr = SIZE_32BIT;
    uint32_t min_access_size_gerrclr = SIZE_8BIT;
    PhysAddr m_reg_addr = 0xFFFFFFFF;
};


/*!
 * @brief Check available write bit..
 */

TEST_F(TRegGERRCLR2, Check_EnWrite_val) {
    EXPECT_CALL(m_treg_gerrstat, SetERR(0x0))
      .Times(AtLeast(1));
    EXPECT_CALL(m_treg_gerrstat, SetOVF(0x0))
      .Times(AtLeast(1));

    // Write all available bit to 1.
    EXPECT_EQ(SYSERR_NONE, m_treg_gerrclr->WriteBody(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT));
}


/*!
 * @brief Check WriteBody method.
 */

TEST_P(TRegGERRCLR2, Check_WriteBody) {
    SideBand cur_sideband = sideband;
    PhysAddr cur_reg_addr = std::get<0>(GetParam());
    uint8_t *data     = std::get<1>(GetParam());
    ByteSize size     = std::get<2>(GetParam());

    EXPECT_CALL(m_treg_gerrstat, SetERR(0x1))
      .Times(AtLeast(1));
    EXPECT_CALL(m_treg_gerrstat, SetOVF(0x1))
      .Times(AtLeast(1));

    // Address of guard error clear.
    if (((cur_reg_addr & 0x3) == 0x0) && ((size == SIZE_32BIT) || (size == SIZE_16BIT) || (size == SIZE_8BIT))) {
        if (((data[0] >> 1) & 0x1) == 1) { // It is written in the same conditions in order to improve the maintainability.
            EXPECT_CALL(m_treg_gerrstat, SetOVF(0x0))
              .Times(AtLeast(1));
        }
        if (((data[0] >> 0) & 0x1) == 1) { // It is written in the same conditions in order to improve the maintainability.
            EXPECT_CALL(m_treg_gerrstat, SetERR(0x0))
              .Times(AtLeast(1));
        }
    }

    m_treg_gerrstat.SetERR(1);
    m_treg_gerrstat.SetOVF(1);

    // Check access size in case TargetWriteDebug method.
    if ((size != SIZE_32BIT) && (size != SIZE_16BIT) && (size != SIZE_8BIT)) {
        ASSERT_THROW(m_treg_gerrclr->WriteBody(cur_reg_addr, data, size), std::range_error);
    } else {
        EXPECT_EQ(SYSERR_NONE, m_treg_gerrclr->WriteBody(cur_reg_addr, data, size));
    }

}

std::tuple<PhysAddr, uint8_t *, uint32_t> const GetSet_ParametersTable2[] = {
    // INPUT_DATA of GERRCLR(PhysAddr, uint8_t *, ByteSize=)
    /* 0000 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_32BIT),
    /* 0001 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT),
    /* 0002 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_16BIT),
    /* 0003 */ std::make_tuple(0x0000cafc, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT),
    /* 0004 */ std::make_tuple(0x0000cafd, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT),
    /* 0005 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT),
    /* 0006 */ std::make_tuple(0x0000caff, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_8BIT),
    /* 0007 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_64BIT),
    /* 0008 */ std::make_tuple(0x0000cafe, new uint8_t[4] {0xFF, 0xFF, 0xFF, 0xFF}, SIZE_128BIT),
    // please write more test case.
};

INSTANTIATE_TEST_CASE_P(Test_WriteBody_WithParameter, TRegGERRCLR2,
                        ::testing::ValuesIn(GetSet_ParametersTable2));


// ----------------------------------------


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
