/**
 * @file test_intc2_tregimr.cpp
 *
 * @date Created on: Dec 07, 2016
 * @author Author: Can Vy <can.vy.wh@rvc.renesas.com>
 * @brief UNIT TEST for TRegIMR.
 * @par Copyright: 2016 Renesas Electronics Corp.
 */
#include <memory>
#include <map>
#include <stdexcept>
#include "mock_intc1.h"
#include "intc2.h"
#include "forest_common.h"
#include "mock_proc_element.h"
#include "mock_sideband.h"
#include "mock_fsbus.h"
#include "gmock/gmock.h"

using ::testing::AtLeast;
using ::testing::_;
using ::testing::Return;

class TRegIMRAccessTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t>> {
protected:
  TRegIMRAccessTest() {}
  virtual ~TRegIMRAccessTest() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    m_mock_fsbus = new MockFsBus();
    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_STANDARD);

    EXPECT_CALL((*m_mock_fsbus), RemoveCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*m_mock_fsbus), AddCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), UpdateIntHighPriority(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), IsIntReqInQueue(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), CancelIntByMask(_)).Times(AtLeast(0));
  }
  virtual void TearDown() {
    delete mock_pe;
    delete m_mock_fsbus;
    delete cintc2;
  }

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *m_mock_fsbus;
  PeVec m_pes;
  CIntc2 *cintc2;
  uint32_t pe_id = 0;
  TRegIMR* imr;
  PhysAddr imr_base_addr = 0xFFF81000UL;
  uint32_t N_G4MH_EIC  = 2048;
  uint32_t N_G4MH_IMR  = N_G4MH_EIC / 32;
  uint32_t N_G4MH_EIBD = 2048;
  uint32_t N_EIC = N_G4MH_EIC;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


/*!
 * @brief Check TRegIMR Reset operation.
 */
TEST_F(TRegIMRAccessTest, CheckIMR_Reset) {
    uint8_t  rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 , 0, 0, 0, 0, 0, 0, 0, 0};
    cintc2->HardReset();
    SCOPED_TRACE("CheckIMR_Reset");
    for (uint32_t i = 1; i < N_G4MH_IMR; i++) {
        imr = new TRegIMR (SIZE_32BIT, SIZE_8BIT, imr_base_addr + i*4, i, cintc2);
        imr->Reset();
        ASSERT_NO_THROW (imr->Read(sideband, imr_base_addr + i*4, rdata, SIZE_32BIT));
        uint32_t expect_data = 0xFFFFFFFFU;
        memcpy (&m_data32, rdata, sizeof(m_data32));
        ASSERT_EQ(expect_data, m_data32);
        delete imr;
    }
}


/*!
 * @brief Check TRegIMR Read/Write operation.
 */
TEST_P(TRegIMRAccessTest, CheckIMR_ReadWrite) {
    PhysAddr addr    = std::get<0>(GetParam());
    int32_t  temp    = static_cast<int32_t>(std::get<1>(GetParam()));
    uint8_t  *wdata  = reinterpret_cast<uint8_t *>(&temp);
    uint8_t  rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    ByteSize size = std::get<2>(GetParam());
    ByteSize expect_size = std::get<3>(GetParam());
    uint32_t expect_value = std::get<4>(GetParam());

    cintc2->HardReset();
    for (uint32_t i = 1; i < N_G4MH_IMR; i++) {
        imr = new TRegIMR (SIZE_32BIT, SIZE_8BIT, imr_base_addr + i*4, i, cintc2);

        imr->Reset();
        {
            SCOPED_TRACE("Write");
            if (size == expect_size) {
                ASSERT_NO_THROW(imr->Write(sideband, addr, wdata, size));
            } else {
                // Expect throw if "Un-guaranteed Size Access".
                ASSERT_THROW(imr->Write(sideband, addr, wdata, size), std::range_error);
            }
        }
        {
            SCOPED_TRACE("Read");
            if (size == expect_size) {
                if (size == SIZE_8BIT) {
                    ASSERT_NO_THROW(imr->Read(sideband, addr, rdata, size));
                    uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (rdata)));
                    ASSERT_EQ(expect_value, m_rdata);
                } else if (size == SIZE_16BIT) {
                    ASSERT_NO_THROW(imr->Read(sideband, addr, rdata, size));
                    uint32_t index_16 = (addr >> 1) & 0x1;
                    memcpy (&m_data16[index_16], rdata, sizeof(uint16_t));
                    ASSERT_EQ(expect_value, m_data16[index_16]);
                } else if (size == SIZE_32BIT) {
                    ASSERT_NO_THROW(imr->Read(sideband, addr, rdata, size));
                    memcpy (&m_data32, rdata, sizeof(m_data32));
                    ASSERT_EQ(expect_value, m_data32);
                }
            } else {
                    // Expect throw if "Un-guaranteed Size Access".
                    ASSERT_THROW(imr->Read(sideband, addr, rdata, size), std::range_error);
            }
        }
        delete imr;
    }
}


std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t> const RW_ParametersTable[] = {
std::make_tuple(0xFFF81080, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0xFF),
std::make_tuple(0xFFF81080, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF81081, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0xFF),
std::make_tuple(0xFFF81081, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF81082, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0xFF),
std::make_tuple(0xFFF81082, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF81083, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0xFF),
std::make_tuple(0xFFF81083, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF81080, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0xFFFF),
std::make_tuple(0xFFF81080, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF81082, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0xFFFF),
std::make_tuple(0xFFF81082, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF81080, 0xFFFFFFFF, SIZE_32BIT, SIZE_32BIT, 0xFFFFFFFF),
std::make_tuple(0xFFF81080, 0x0, SIZE_32BIT, SIZE_32BIT, 0x0),
std::make_tuple(0xFFF81080, 0x0, SIZE_64BIT, SIZE_32BIT, 0x0),
std::make_tuple(0xFFF81080, 0xFFFFFFFF, SIZE_64BIT, SIZE_32BIT, 0x0),
};

INSTANTIATE_TEST_CASE_P(TestWithParameters1, TRegIMRAccessTest,
                        ::testing::ValuesIn(RW_ParametersTable));


class TRegIMRAccessTestDebug
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t>> {
protected:
  TRegIMRAccessTestDebug() {}
  virtual ~TRegIMRAccessTestDebug() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    m_mock_fsbus = new MockFsBus();
    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_STANDARD);

    EXPECT_CALL((*m_mock_fsbus), RemoveCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*m_mock_fsbus), AddCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), UpdateIntHighPriority(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), IsIntReqInQueue(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), CancelIntByMask(_)).Times(AtLeast(0));
  }
  virtual void TearDown() {
    delete mock_pe;
    delete m_mock_fsbus;
    delete cintc2;
  }

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *m_mock_fsbus;
  PeVec m_pes;
  CIntc2 *cintc2;
  uint32_t pe_id = 0;
  TRegIMR* imr;
  PhysAddr imr_base_addr = 0xFFF81000UL;
  uint32_t N_G4MH_EIC  = 2048;
  uint32_t N_G4MH_IMR  = N_G4MH_EIC / 32;
  uint32_t N_G4MH_EIBD = 2048;
  uint32_t N_EIC = N_G4MH_EIC;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


/*!
 * @brief Check TRegIMR Read/Write in debug operation.
 */
TEST_P(TRegIMRAccessTestDebug, CheckIMR_ReadWriteDebug) {
    PhysAddr addr    = std::get<0>(GetParam());
    int32_t  temp    = static_cast<int32_t>(std::get<1>(GetParam()));
    uint8_t  *wdata  = reinterpret_cast<uint8_t *>(&temp);
    uint8_t  rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    ByteSize size = std::get<2>(GetParam());
    ByteSize expect_size = std::get<3>(GetParam());
    uint32_t expect_value = std::get<4>(GetParam());

    for (uint32_t i = 1; i < N_G4MH_IMR; i++) {
        imr = new TRegIMR (SIZE_32BIT, SIZE_8BIT, imr_base_addr + i*4, i, cintc2);

        imr->Reset();
        {
            SCOPED_TRACE("Write");
            if (size == expect_size) {
                ASSERT_NO_THROW(imr->WriteBody(addr, wdata, size));
            } else {
                // Expect throw if "Un-guaranteed Size Access".
                ASSERT_THROW(imr->WriteBody(addr, wdata, size), std::range_error);
            }
        }
        {
            SCOPED_TRACE("Read");
            if (size == expect_size) {
                if (size == SIZE_8BIT) {
                    ASSERT_NO_THROW(imr->ReadBody(addr, rdata, size));
                    uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (rdata)));
                    ASSERT_EQ(expect_value, m_rdata);
                } else if (size == SIZE_16BIT) {
                    ASSERT_NO_THROW(imr->ReadBody(addr, rdata, size));
                    uint32_t index_16 = (addr >> 1) & 0x1;
                    memcpy (&m_data16[index_16], rdata, sizeof(uint16_t));
                    ASSERT_EQ(expect_value, m_data16[index_16]);
                } else if (size == SIZE_32BIT) {
                    ASSERT_NO_THROW(imr->ReadBody(addr, rdata, size));
                    memcpy (&m_data32, rdata, sizeof(m_data32));
                    ASSERT_EQ(expect_value, m_data32);
                }
            } else {
                    // Expect throw if "Un-guaranteed Size Access".
                    ASSERT_THROW(imr->ReadBody(addr, rdata, size), std::range_error);
            }
        }
        delete imr;
    }
}


std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t> const RW_ParametersTable_2[] = {
std::make_tuple(0xFFF81080, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0xFF),
std::make_tuple(0xFFF81080, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF81081, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0xFF),
std::make_tuple(0xFFF81081, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF81082, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0xFF),
std::make_tuple(0xFFF81082, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF81083, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0xFF),
std::make_tuple(0xFFF81083, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF81080, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0xFFFF),
std::make_tuple(0xFFF81080, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF81082, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0xFFFF),
std::make_tuple(0xFFF81082, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF81080, 0xFFFFFFFF, SIZE_32BIT, SIZE_32BIT, 0xFFFFFFFF),
std::make_tuple(0xFFF81080, 0x0, SIZE_32BIT, SIZE_32BIT, 0x0),
};

INSTANTIATE_TEST_CASE_P(TestWithParameters1, TRegIMRAccessTestDebug,
                        ::testing::ValuesIn(RW_ParametersTable_2));


class TRegIMRFunction
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t>> {
protected:
  TRegIMRFunction() {}
  virtual ~TRegIMRFunction() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    m_mock_fsbus = new MockFsBus();
    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_STANDARD);

    EXPECT_CALL((*m_mock_fsbus), RemoveCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*m_mock_fsbus), AddCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), UpdateIntHighPriority(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), IsIntReqInQueue(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), CancelIntByMask(_)).Times(AtLeast(0));
  }
  virtual void TearDown() {
    delete mock_pe;
    delete m_mock_fsbus;
    delete cintc2;
  }

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *m_mock_fsbus;
  PeVec m_pes;
  CIntc2 *cintc2;
  uint32_t pe_id = 0;
  TRegIMR* imr;
  PhysAddr imr_base_addr = 0xFFF81000UL;
  uint32_t N_G4MH_EIC  = 2048;
  uint32_t N_G4MH_IMR  = N_G4MH_EIC / 32;
  uint32_t N_G4MH_EIBD = 2048;
  uint32_t N_EIC = N_G4MH_EIC;

private:
};


/*!
 * @brief Check TRegIMR GetName operation.
 */
TEST_F(TRegIMRFunction, CheckIMR_GetName) {
    SCOPED_TRACE("CheckIMR_Reset");
    for (uint32_t i = 1; i < N_G4MH_IMR; i++) {
        imr = new TRegIMR (SIZE_32BIT, SIZE_8BIT, imr_base_addr + i*4, i, cintc2);
        imr->Reset();
        const char* name = imr->GetName();
        ASSERT_STREQ("IMR", name);
        delete imr;
    }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
