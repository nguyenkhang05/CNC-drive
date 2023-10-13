/**
 * @file test_intc2_tregeic_intc2.cpp
 *
 * @date Created on: Dec 07, 2016
 * @author Author: Can Vy <can.vy.wh@rvc.renesas.com>
 * @brief UNIT TEST for TRegEIC.
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

class TRegEICAccessTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t>> {
protected:
  TRegEICAccessTest() {}
  virtual ~TRegEICAccessTest() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    m_mock_fsbus = new MockFsBus();
    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_STANDARD);

    EXPECT_CALL((*m_mock_fsbus), RemoveCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*m_mock_fsbus), AddCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), UpdateIntHighPriority(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), IsIntReqInQueue(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), CancelIntByIntc2(_)).Times(AtLeast(0));
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
  TRegEIC* eic;
  uint32_t pe_id = 0;
  PhysAddr eic_base_addr = 0xFFF80000UL;
  uint32_t eic_num = 2048;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


/*!
 * @brief Check TRegEIC Reset operation.
 */
TEST_F(TRegEICAccessTest, CheckEIC_Reset) {
    uint8_t  rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    cintc2->HardReset();
    SCOPED_TRACE("CheckEIC_Reset");
    for (uint32_t i = 32; i < eic_num; i++) {
        eic = new TRegEIC (SIZE_16BIT, SIZE_8BIT, eic_base_addr + i * 2, i, pe_id, cintc2);
        eic->Reset();
        ASSERT_NO_THROW (eic->Read(sideband, eic_base_addr + i*2, rdata, SIZE_16BIT));
#if _INTC1_20
        uint16_t expect_data = 0x008F;
#else
        uint16_t expect_data = 0x008F;
#endif
        uint32_t index_16 = ((eic_base_addr + i*2) >> 1) & 0x1;
        memcpy (&m_data16[index_16], rdata, sizeof(uint16_t));
        EXPECT_EQ(expect_data, m_data16[index_16]);
        delete eic;
    }
}


/*!
 * @brief Check TRegEIC Read/Write operation.
 */
TEST_P(TRegEICAccessTest, CheckEIC_ReadWrite) {
    PhysAddr addr    = std::get<0>(GetParam());
    int32_t  temp    = static_cast<int32_t>(std::get<1>(GetParam()));
    uint8_t  *wdata  = reinterpret_cast<uint8_t *>(&temp);
    uint8_t  rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    ByteSize size = std::get<2>(GetParam());
    ByteSize expect_size = std::get<3>(GetParam());
    uint32_t expect_value = std::get<4>(GetParam());

    for (uint32_t i = 32; i < eic_num; i++) {
        eic = new TRegEIC (SIZE_16BIT, SIZE_8BIT, eic_base_addr + i * 2, i, pe_id, cintc2);
        eic->Reset();
        {
            SCOPED_TRACE("Write");
            if (size == expect_size) {
                ASSERT_NO_THROW(eic->Write(sideband, addr, wdata, size));
            } else {
                // Expect throw if "Un-guaranteed Size Access".
                ASSERT_THROW(eic->Write(sideband, addr, wdata, size), std::range_error);
            }
        }
        {
            SCOPED_TRACE("Read");
            if (size == expect_size) {
                if (size == SIZE_8BIT) {
                    ASSERT_NO_THROW(eic->Read(sideband, addr, rdata, size));
                    uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (rdata)));
                    ASSERT_EQ(expect_value, m_rdata);
                } else if (size == SIZE_16BIT) {
                    ASSERT_NO_THROW(eic->Read(sideband, addr, rdata, size));
                    uint32_t index_16 = ( addr >> 1) & 0x1;
                    memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
                    ASSERT_EQ(expect_value, m_data16[index_16]);
                } else if (size == SIZE_32BIT) {
                    ASSERT_NO_THROW(eic->Read(sideband, addr, rdata, size));
                    memcpy (&m_data32, rdata, sizeof(m_data32));
                    ASSERT_EQ(expect_value, m_data32);
                }
            } else {
                    // Expect throw if "Un-guaranteed Size Access".
                    ASSERT_THROW(eic->Read(sideband, addr, rdata, size), std::range_error);
            }
        }
        delete eic;
    }
}


std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t> const RW_ParametersTable[] = {
std::make_tuple(0xFFF80040, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0xCF),
std::make_tuple(0xFFF80040, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF80041, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x10),
std::make_tuple(0xFFF80041, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF80040, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x10CF),
std::make_tuple(0xFFF80040, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF80040, 0x0, SIZE_32BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF80040, 0xFFFFFFFF, SIZE_32BIT, SIZE_16BIT, 0x0),
};

INSTANTIATE_TEST_CASE_P(TestWithParameters1, TRegEICAccessTest,
                        ::testing::ValuesIn(RW_ParametersTable));


class TRegEICAccessTestDebug
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t>> {
protected:
  TRegEICAccessTestDebug() {}
  virtual ~TRegEICAccessTestDebug() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    m_mock_fsbus = new MockFsBus();
    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_STANDARD);

    EXPECT_CALL((*m_mock_fsbus), RemoveCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*m_mock_fsbus), AddCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), UpdateIntHighPriority(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), IsIntReqInQueue(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), CancelIntByIntc2(_)).Times(AtLeast(0));
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
  TRegEIC* eic;
  uint32_t pe_id = 0;
  PhysAddr eic_base_addr = 0xFFF80000UL;
  uint32_t eic_num = 2048;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


/*!
 * @brief Check TRegEIC Read/Write in debug operation.
 */
TEST_P(TRegEICAccessTestDebug, CheckEIC_ReadWriteBody) {
    PhysAddr addr    = std::get<0>(GetParam());
    int32_t  temp    = static_cast<int32_t>(std::get<1>(GetParam()));
    uint8_t  *wdata  = reinterpret_cast<uint8_t *>(&temp);
    uint8_t  rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    ByteSize size = std::get<2>(GetParam());
    ByteSize expect_size = std::get<3>(GetParam());
    uint32_t expect_value = std::get<4>(GetParam());

    for (uint32_t i = 32; i < eic_num; i++) {
        eic = new TRegEIC (SIZE_16BIT, SIZE_8BIT, eic_base_addr + i * 2, i, pe_id, cintc2);
        eic->Reset();
        {
            SCOPED_TRACE("Write");
            if (size == expect_size) {
                ASSERT_NO_THROW(eic->WriteBody(addr, wdata, size));
            } else {
                // Expect throw if "Un-guaranteed Size Access".
                ASSERT_THROW(eic->WriteBody(addr, wdata, size), std::range_error);
            }
        }
        {
            SCOPED_TRACE("Read");
            if (size == expect_size) {
                if (size == SIZE_8BIT) {
                    ASSERT_NO_THROW(eic->ReadBody(addr, rdata, size));
                    uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (rdata)));
                    ASSERT_EQ(expect_value, m_rdata);
                } else if (size == SIZE_16BIT) {
                    ASSERT_NO_THROW(eic->ReadBody(addr, rdata, size));
                    uint32_t index_16 = ( addr >> 1) & 0x1;
                    memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
                    ASSERT_EQ(expect_value, m_data16[index_16]);
                } else if (size == SIZE_32BIT) {
                    ASSERT_NO_THROW(eic->ReadBody(addr, rdata, size));
                    memcpy (&m_data32, rdata, sizeof(m_data32));
                    ASSERT_EQ(expect_value, m_data32);
                }
            } else {
                    // Expect throw if "Un-guaranteed Size Access".
                    ASSERT_THROW(eic->ReadBody(addr, rdata, size), std::range_error);
            }
        }
        delete eic;
    }
}

std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t> const RW_ParametersTable_2[] = {
std::make_tuple(0xFFF80040, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0xCF),
std::make_tuple(0xFFF80040, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF80041, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x90),
std::make_tuple(0xFFF80041, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF80040, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x90CF),
std::make_tuple(0xFFF80040, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
};

INSTANTIATE_TEST_CASE_P(TestWithParameters1, TRegEICAccessTestDebug,
                        ::testing::ValuesIn(RW_ParametersTable_2));


class TRegEICBitSetTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t>> {
protected:
  TRegEICBitSetTest() {}
  virtual ~TRegEICBitSetTest() {}
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
    EXPECT_CALL((*mock_pe), CancelIntByIntc2(_)).Times(AtLeast(0));
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
  TRegEIC* eic;
  uint32_t pe_id = 0;
  PhysAddr eic_base_addr = 0xFFF80000UL;
  uint32_t eic_num = 2048;

private:
};


/*!
 * @brief Check TRegEIC Get/Set EICT operation.
 */
TEST_F(TRegEICBitSetTest, CheckEIC_EICT) {
    uint32_t expect_data_low = 0x0;
    uint32_t expect_data_hi = 0x1;
    SCOPED_TRACE("CheckEIC_EICT");
    cintc2->HardReset();
    eic = new TRegEIC (SIZE_16BIT, SIZE_8BIT, eic_base_addr + 0x20*2, 0x20, pe_id, cintc2); //Channel 32
    eic->Reset();
    ASSERT_NO_THROW(eic->SetEICT(0));
    ASSERT_NO_THROW(eic->GetEICT());
    ASSERT_EQ(expect_data_low, eic->GetEICT());

    ASSERT_NO_THROW(eic->SetEICT(1));
    ASSERT_NO_THROW(eic->GetEICT());
    ASSERT_EQ(expect_data_hi, eic->GetEICT());
    delete eic;
}


/*!
 * @brief Check TRegEIC Get/Set EITB operation.
 */
TEST_F(TRegEICBitSetTest, CheckEIC_EITB) {
    uint32_t expect_data_low = 0x0;
    uint32_t expect_data_hi = 0x1;
    SCOPED_TRACE("CheckEIC_EITB");
    cintc2->HardReset();
    eic = new TRegEIC (SIZE_16BIT, SIZE_8BIT, eic_base_addr + 0x20*2, 0x20, pe_id, cintc2); //Channel 32
    eic->Reset();
    ASSERT_NO_THROW(eic->SetEITB(0));
    ASSERT_NO_THROW(eic->GetEITB());
    ASSERT_EQ(expect_data_low, eic->GetEITB());

    ASSERT_NO_THROW(eic->SetEITB(1));
    ASSERT_NO_THROW(eic->GetEITB());
    ASSERT_EQ(expect_data_hi, eic->GetEITB());
    delete eic;
}


/*!
 * @brief Check TRegEIC Get/Set EIRF operation.
 */
TEST_F(TRegEICBitSetTest, CheckEIC_EIRF) {
    uint32_t expect_data_low = 0x0;
    uint32_t expect_data_hi = 0x1;
    SCOPED_TRACE("CheckEIC_EIRF");
    cintc2->HardReset();
    eic = new TRegEIC (SIZE_16BIT, SIZE_8BIT, eic_base_addr + 0x20*2, 0x20, pe_id, cintc2); //Channel 32
    eic->Reset();
    ASSERT_NO_THROW(eic->SetEIRF(0));
    ASSERT_NO_THROW(eic->GetEIRF());
    ASSERT_EQ(expect_data_low, eic->GetEIRF());

    ASSERT_NO_THROW(eic->SetEIRF(1));
    ASSERT_NO_THROW(eic->GetEIRF());
    ASSERT_EQ(expect_data_hi, eic->GetEIRF());
    delete eic;
}


/*!
 * @brief Check TRegEIC Get/Set EIMK operation.
 */
TEST_F(TRegEICBitSetTest, CheckEIC_EIMK) {
    uint32_t expect_data_low = 0x0;
    uint32_t expect_data_hi = 0x1;
    SCOPED_TRACE("CheckEIC_EIMK");
    cintc2->HardReset();
    eic = new TRegEIC (SIZE_16BIT, SIZE_8BIT, eic_base_addr + 0x20*2, 0x20, pe_id, cintc2); //Channel 32
    eic->Reset();
    ASSERT_NO_THROW(eic->SetEIMK(0));
    ASSERT_NO_THROW(eic->GetEIMK());
    ASSERT_EQ(expect_data_low, eic->GetEIMK());

    ASSERT_NO_THROW(eic->SetEIMK(1));
    ASSERT_NO_THROW(eic->GetEIMK());
    ASSERT_EQ(expect_data_hi, eic->GetEIMK());
    delete eic;
}


/*!
 * @brief Check TRegEIC Get/Set EIP operation.
 */
TEST_F(TRegEICBitSetTest, CheckEIC_EIP) {
    SCOPED_TRACE("CheckEIC_EIP");
    cintc2->HardReset();
    eic = new TRegEIC (SIZE_16BIT, SIZE_8BIT, eic_base_addr + 0x20*2, 0x20, pe_id, cintc2); //Channel 32
    eic->Reset();
    for (uint32_t i = 0; i < 16; i++) {
        ASSERT_NO_THROW(eic->SetEIP(i));
        ASSERT_NO_THROW(eic->GetEIP());
        ASSERT_EQ(i, eic->GetEIP());
    }
    delete eic;
}


/*!
 * @brief Check TRegEIC SetBindCST/SetBindPEID operation.
 */
TEST_F(TRegEICBitSetTest, CheckEIC_SetBindCST_SetBindPEID) {
    SCOPED_TRACE("CheckEIC_SetBindCST_SetBindPEID");
    cintc2->HardReset();
    eic = new TRegEIC (SIZE_16BIT, SIZE_8BIT, eic_base_addr + 0x20*2, 0x20, pe_id, cintc2); //Channel 32
    eic->Reset();
    ASSERT_NO_THROW(eic->SetBindCST(1));
    ASSERT_EQ(0x1U, eic->m_bind_cst);
    ASSERT_NO_THROW(eic->SetBindPEID(7));
    ASSERT_EQ(0x7U, eic->m_bind_peid);
    delete eic;
}


/*!
 * @brief Check TRegEIC GetName operation.
 */
TEST_F(TRegEICBitSetTest, Check_GetName) {
    SCOPED_TRACE("Check_GetName");
    cintc2->HardReset();
    eic = new TRegEIC (SIZE_16BIT, SIZE_8BIT, eic_base_addr + 0x20*2, 0x20, pe_id, cintc2); //Channel 32
    eic->Reset();
    const char* name = eic->GetName();
    ASSERT_STREQ("EIC", name);
    delete eic;
}


/*!
 * @brief Check TRegEIC UpdateEICHighPriority operation.
 */
TEST_F(TRegEICBitSetTest, CheckEIC_UpdateEICHighPriority_1) {
    SCOPED_TRACE("CheckEIC_UpdateEICHighPriority_1");
    uint32_t eic_addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t eic_data = 0x1000U;      /*EIC.EIRF=1*/
    uint8_t  *eic_wdata  = reinterpret_cast<uint8_t *>(&eic_data);
    cintc2->HardReset();
    eic = new TRegEIC (SIZE_16BIT, SIZE_8BIT, eic_base_addr + 0x20*2, 0x20, pe_id, cintc2); //Channel 32
    eic->Reset();

    eic->Write (sideband, eic_addr, eic_wdata, SIZE_16BIT);

    eic->m_channel = 32;
    eic->m_bind_peid = 0;
    ASSERT_NO_THROW(eic->UpdateEICHighPriority());
    ASSERT_EQ(0x20U, cintc2->m_intc2_priority[pe_id]->GetChannel());
    ASSERT_EQ(0x00U, eic->GetEIP ());

    delete eic;
}


/*!
 * @brief Check TRegEIC UpdateEICHighPriority operation.
 */
TEST_F(TRegEICBitSetTest, CheckEIC_UpdateEICHighPriority_2) {
    SCOPED_TRACE("CheckEIC_UpdateEICHighPriority_2");
    uint32_t eic_addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t eic_data = 0x1000U;      /*EIC.EIRF=1*/
    uint8_t  *eic_wdata  = reinterpret_cast<uint8_t *>(&eic_data);
    eic = new TRegEIC (SIZE_16BIT, SIZE_8BIT, eic_base_addr + 0x20*2, 0x20, pe_id, cintc2); //Channel 32
    eic->Reset();

    eic->Write (sideband, eic_addr, eic_wdata, SIZE_16BIT);

    uint16_t eic_data_2 = 0x1007U;      /*EIC.EIRF=1*/
    uint8_t  *eic_wdata_2  = reinterpret_cast<uint8_t *>(&eic_data_2);
    eic->Write(sideband, eic_addr, eic_wdata_2, SIZE_16BIT);

    eic->m_channel = 32;
    eic->m_bind_peid = 0;
    ASSERT_NO_THROW(eic->UpdateEICHighPriority());
    delete eic;
}


class TRegEIC_TestReplaceEICWriteData
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t>> {
protected:
  TRegEIC_TestReplaceEICWriteData() {}
  virtual ~TRegEIC_TestReplaceEICWriteData() {}
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
    EXPECT_CALL((*mock_pe), CancelIntByIntc2(_)).Times(AtLeast(0));
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
  TRegEIC* eic;
  uint32_t pe_id = 0;
  PhysAddr eic_base_addr = 0xFFF80000UL;
  uint32_t eic_num = 2048;

private:
};


/*!
 * @brief Check TRegEIC ReplaceEICWriteData operation.
 */
TEST_F(TRegEIC_TestReplaceEICWriteData, CheckEIC_ReplaceEICWriteData) {
    uint8_t  wdata[]  = {0xFF, 0xFF};
    SCOPED_TRACE("CheckEIC_ReplaceEICWriteData");
    eic = new TRegEIC (SIZE_16BIT, SIZE_8BIT, eic_base_addr + 0x20*2, 0x20, pe_id, cintc2); //Channel 32
    eic->Reset();
    ASSERT_NO_THROW(eic->SetEIRF(1));
    ASSERT_NO_THROW(eic->SetEICT(0));
    eic->ReplaceEICWriteData(wdata, 0);
    ASSERT_EQ(0x7FU, wdata[0]);
    ASSERT_NO_THROW(eic->SetEICT(1));
    eic->ReplaceEICWriteData(wdata, 1);
    ASSERT_EQ(0xFFU, wdata[1]);
    delete eic;
}


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}

