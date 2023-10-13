/*
 * @ test_tregbnd.cpp
 *
 * @date Created on: Dec 10, 2016
 * @author Author: Can Vy <can.vy.wh@rvc.renesas.com>
 * @brief UNIT TEST for TregBND.
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

class TregBND_EIBDTest : public testing::Test,
                         public testing::WithParamInterface<std::tuple<PhysAddr, uint32_t, ByteSize, int32_t, uint32_t>> {
protected:
  TregBND_EIBDTest() {}
  virtual ~TregBND_EIBDTest() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_20, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    m_mock_fsbus = new MockFsBus();
    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_G4MH20);
    TRegMask mask = TRegMask (TRegBND<EIBD>::INTC2_EIBD_MASK_G4MH20);
    bnd_eibd = new TRegBND<EIBD>(SIZE_32BIT, SIZE_8BIT, addr, mask, channel, pe_id, cintc2, INTC2_G4MH20);
  }
  virtual void TearDown() {
    delete mock_pe;
    delete m_mock_fsbus;
    delete cintc2;
    delete bnd_eibd;
  }

public:
  uint32_t pe_id = 0;
  uint32_t channel = 32;
  uint32_t addr = 0xFFF82080;
  TRegBND<EIBD>* bnd_eibd;
  MockProcElement *mock_pe;
  MockFsBus *m_mock_fsbus;
  PeVec m_pes;
  CIntc2 *cintc2;
  MockSideBand sideband;

private:
};


/*!
 * @brief Check TregBND_EIBD IsRegWriteEnable operation.
 */
TEST_F(TregBND_EIBDTest, Check_IsRegWriteEnable) {
    SCOPED_TRACE("Check_IsRegWriteEnable");
    EXPECT_EQ(true, bnd_eibd->IsRegWriteEnable(sideband));
}


/*!
 * @brief Check TregBND_EIBD IsIntc1EIBD operation.
 */
TEST_F(TregBND_EIBDTest, Check_IsIntc1EIBD) {
    SCOPED_TRACE("Check_IsIntc1EIBD");
    bnd_eibd->m_channel = 0;
    EXPECT_EQ(true, bnd_eibd->IsIntc1EIBD());
    bnd_eibd->m_channel = 32;
    EXPECT_NE(true, bnd_eibd->IsIntc1EIBD());
}


/*!
 * @brief Check TregBND_EIBD IsIntc2EIBD operation.
 */
TEST_F(TregBND_EIBDTest, Check_IsIntc2EIBD) {
    SCOPED_TRACE("Check_IsIntc2EIBD");
    bnd_eibd->m_channel = channel;
    EXPECT_EQ(true, bnd_eibd->IsIntc2EIBD());
    bnd_eibd->m_channel = 0;
    EXPECT_NE(true, bnd_eibd->IsIntc2EIBD());
}


/*!
 * @brief Check TregBND_EIBD IsIntLevelDetect operation.
 */
TEST_F(TregBND_EIBDTest, Check_IsIntLevelDetect) {
    SCOPED_TRACE("Check_IsIntLevelDetect");
    cintc2->GetEICPtr(channel)->SetEICT(1);
    EXPECT_EQ(true, bnd_eibd->IsIntLevelDetect(channel));
    cintc2->GetEICPtr(channel)->SetEICT(0);
    EXPECT_NE(true, bnd_eibd->IsIntLevelDetect(channel));
}


/*!
 * @brief Check TregBND_EIBD Get/SetCST operation.
 */
TEST_F(TregBND_EIBDTest, Check_GetSetCST) {
    SCOPED_TRACE("Check_GetSetCST");
    bnd_eibd->SetCST(0);
    EXPECT_EQ(0x0U, bnd_eibd->GetCST());
    bnd_eibd->SetCST(1);
    EXPECT_EQ(0x1U, bnd_eibd->GetCST());
}


/*!
 * @brief Check TregBND_EIBD Get/SetBCP operation.
 */
TEST_F(TregBND_EIBDTest, Check_GetSetBCP) {
    SCOPED_TRACE("Check_GetSetBCP");
    bnd_eibd->SetBCP(0);
    EXPECT_EQ(0x0U, bnd_eibd->GetBCP());
    bnd_eibd->SetBCP(1);
    EXPECT_EQ(0x1U, bnd_eibd->GetBCP());
    bnd_eibd->SetBCP(2);
    EXPECT_EQ(0x2U, bnd_eibd->GetBCP());
    bnd_eibd->SetBCP(3);
    EXPECT_EQ(0x3U, bnd_eibd->GetBCP());
}


/*!
 * @brief Check TregBND_EIBD Get/SetPEID operation.
 */
TEST_F(TregBND_EIBDTest, Check_GetSetPEID) {
    SCOPED_TRACE("Check_GetSetPEID");
    bnd_eibd->SetPEID(0);
    EXPECT_EQ(0x0U, bnd_eibd->GetPEID());
    bnd_eibd->SetPEID(15);
    EXPECT_EQ(0x7U, bnd_eibd->GetPEID());
}


/*!
 * @brief Check TregBND_EIBD GetName operation.
 */
TEST_F(TregBND_EIBDTest, Check_GetName) {
    SCOPED_TRACE("Check_GetName");
    const char* name = bnd_eibd->GetName();
    ASSERT_STREQ("EIBD", name);
}


/*!
 * @brief Check TregBND_EIBD SetBind operation.
 */
TEST_F(TregBND_EIBDTest, Check_SetBind) {
    SCOPED_TRACE("Check_SetBind");
    bnd_eibd->m_channel = channel;
    TRegEIC* eic = cintc2->GetEICPtr (bnd_eibd->m_channel);
    bnd_eibd->SetCST(0);
    bnd_eibd->SetPEID(0);
    bnd_eibd->SetBind();
    EXPECT_EQ(0x0U, eic->m_bind_cst);
    EXPECT_EQ(0x0U, eic->m_bind_peid);
    bnd_eibd->SetCST(1);
    bnd_eibd->SetPEID(15);
    bnd_eibd->SetBind();
    EXPECT_EQ(0x1U, eic->m_bind_cst);
    EXPECT_EQ(0x7U, eic->m_bind_peid);
}


/*!
 * @brief Check TregBND_EIBD WriteBodyByte operation.
 */
TEST_P(TregBND_EIBDTest, Check_WriteBodyByte) {
    PhysAddr address    = std::get<0>(GetParam());
    uint32_t temp    = static_cast<uint32_t>(std::get<1>(GetParam()));
    uint8_t  *data   = reinterpret_cast<uint8_t *>(&temp);
    uint8_t  rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    uint8_t  wdata   = *data;

    ByteSize size         = std::get<2>(GetParam());
    ByteSize expect_value = std::get<3>(GetParam());
    uint32_t  pos         = std::get<4>(GetParam());
    bnd_eibd->m_channel = channel;
    cintc2->GetEICPtr(channel)->SetEICT(0);

    SCOPED_TRACE("Check_WriteBodyByte");
    bnd_eibd->WriteBodyByte(pos, wdata);
    bnd_eibd->ReadBody(address, rdata, size);
    uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (rdata)));
    EXPECT_EQ(expect_value, m_rdata);
}

std::tuple<PhysAddr, uint32_t, ByteSize, int32_t, uint32_t> const RW_ParametersTable[] = {
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_8BIT, 0x07, 0x0),
std::make_tuple(0xFFF82080, 0x0, SIZE_8BIT, 0x0, 0x0),
std::make_tuple(0xFFF82081, 0xFFFFFFFF, SIZE_8BIT, 0x87, 0x1),
std::make_tuple(0xFFF82081, 0x0, SIZE_8BIT, 0x0, 0x1),
std::make_tuple(0xFFF82082, 0xFFFFFFFF, SIZE_8BIT, 0x0, 0x2),
std::make_tuple(0xFFF82082, 0x0, SIZE_8BIT, 0x0, 0x2),
std::make_tuple(0xFFF82083, 0xFFFFFFFF, SIZE_8BIT, 0x83, 0x3),
std::make_tuple(0xFFF82083, 0x0, SIZE_8BIT, 0x0, 0x3),
};

INSTANTIATE_TEST_CASE_P(TestWithParameters, TregBND_EIBDTest,
                        ::testing::ValuesIn(RW_ParametersTable));


class TregBND_EIBDTest_2 : public testing::Test,
                         public testing::WithParamInterface<std::tuple<PhysAddr, uint32_t, ByteSize, ByteSize, int32_t>> {
protected:
  TregBND_EIBDTest_2() {}
  virtual ~TregBND_EIBDTest_2() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_20, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    m_mock_fsbus = new MockFsBus();
    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_G4MH20);
    TRegMask mask = TRegMask (TRegBND<EIBD>::INTC2_EIBD_MASK_G4MH20);
    bnd_eibd = new TRegBND<EIBD>(SIZE_32BIT, SIZE_8BIT, addr, mask, channel, pe_id, cintc2, INTC2_G4MH20);
    EXPECT_CALL((*m_mock_fsbus), AddCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), CancelIntByIntc2(_)).Times(AtLeast(0));
  }
  virtual void TearDown() {
    delete mock_pe;
    delete m_mock_fsbus;
    delete cintc2;
    delete bnd_eibd;
  }

public:
  uint32_t pe_id = 0;
  uint32_t channel = 32;
  uint32_t addr = 0xFFF82080;
  TRegBND<EIBD>* bnd_eibd;
  MockProcElement *mock_pe;
  MockFsBus *m_mock_fsbus;
  PeVec m_pes;
  CIntc2 *cintc2;
  MockSideBand sideband;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


/*!
 * @brief Check TregBND_EIBD WriteBodyByte operation.
 */
TEST_P(TregBND_EIBDTest_2, Check_WriteBody) {
    PhysAddr address = std::get<0>(GetParam());
    int32_t  temp    = static_cast<int32_t>(std::get<1>(GetParam()));
    uint8_t  *wdata  = reinterpret_cast<uint8_t *>(&temp);
    uint8_t  rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    ByteSize size = std::get<2>(GetParam());
    ByteSize expect_size = std::get<3>(GetParam());
    uint32_t expect_value = std::get<4>(GetParam());
    bnd_eibd->m_channel = channel;
    cintc2->GetEICPtr(channel)->SetEICT(0);

    SCOPED_TRACE("Check_WriteBody");

    if (size == expect_size) {
      if (size == SIZE_8BIT) {
          bnd_eibd->WriteBody(address, wdata, size);
          bnd_eibd->ReadBody(address, rdata, size);
          uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (rdata)));
          ASSERT_EQ(expect_value, m_rdata);
      } else if (size == SIZE_16BIT) {
          bnd_eibd->WriteBody(address, wdata, size);
          bnd_eibd->ReadBody(address, rdata, size);
          uint32_t index_16 = (address >> 1) & 0x1;
          memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
          ASSERT_EQ(expect_value, m_data16[index_16]);
      } else if (size == SIZE_32BIT) {
          bnd_eibd->WriteBody(address, wdata, size);
          bnd_eibd->ReadBody(address, rdata, size);
          memcpy (&m_data32, rdata, sizeof(m_data32));
          ASSERT_EQ(expect_value, m_data32);
      }
    }
}

std::tuple<PhysAddr, uint32_t, ByteSize, ByteSize, int32_t> const RW_ParametersTable_2[] = {
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x07),
std::make_tuple(0xFFF82080, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82081, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x87),
std::make_tuple(0xFFF82081, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82082, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x00),
std::make_tuple(0xFFF82082, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82083, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x83),
std::make_tuple(0xFFF82083, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x8707),
std::make_tuple(0xFFF82080, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF82082, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x8300),
std::make_tuple(0xFFF82082, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_32BIT, SIZE_32BIT, 0x83008707),
std::make_tuple(0xFFF82080, 0x0, SIZE_32BIT, SIZE_32BIT, 0x0),
};

INSTANTIATE_TEST_CASE_P(TestWithParameters2, TregBND_EIBDTest_2,
                        ::testing::ValuesIn(RW_ParametersTable_2));


class TregBND_EIBDTest_3 : public testing::Test,
                         public testing::WithParamInterface<std::tuple<PhysAddr, uint32_t, ByteSize, ByteSize, int32_t>> {
protected:
  TregBND_EIBDTest_3() {}
  virtual ~TregBND_EIBDTest_3() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_20, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    m_mock_fsbus = new MockFsBus();
    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_G4MH20);
    TRegMask mask = TRegMask (TRegBND<EIBD>::INTC2_EIBD_MASK_G4MH20);
    bnd_eibd = new TRegBND<EIBD>(SIZE_32BIT, SIZE_8BIT, addr, mask, channel, pe_id, cintc2, INTC2_G4MH20);
    EXPECT_CALL((*m_mock_fsbus), AddCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), CancelIntByIntc2(_)).Times(AtLeast(0));
  }
  virtual void TearDown() {
    delete mock_pe;
    delete m_mock_fsbus;
    delete cintc2;
    delete bnd_eibd;
  }

public:
  uint32_t pe_id = 0;
  uint32_t channel = 32;
  uint32_t addr = 0xFFF82080;
  TRegBND<EIBD>* bnd_eibd;
  MockProcElement *mock_pe;
  MockFsBus *m_mock_fsbus;
  PeVec m_pes;
  CIntc2 *cintc2;
  MockSideBand sideband;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


/*!
 * @brief Check TregBND_EIBD Read/Write operation.
 */
TEST_P(TregBND_EIBDTest_3, Check_ReadWrite) {
    PhysAddr address = std::get<0>(GetParam());
    int32_t  temp    = static_cast<int32_t>(std::get<1>(GetParam()));
    uint8_t  *wdata  = reinterpret_cast<uint8_t *>(&temp);
    uint8_t  rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

    ByteSize size = std::get<2>(GetParam());
    ByteSize expect_size = std::get<3>(GetParam());
    uint32_t expect_value = std::get<4>(GetParam());
    bnd_eibd->m_channel = channel;
    cintc2->GetEICPtr(channel)->SetEICT(0);

    SCOPED_TRACE("Check_WriteBody");

    if (size == expect_size) {
      if (size == SIZE_8BIT) {
          bnd_eibd->Write(sideband, address, wdata, size);
          bnd_eibd->Read(sideband, address, rdata, size);
          uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (rdata)));
          ASSERT_EQ(expect_value, m_rdata);
      } else if (size == SIZE_16BIT) {
          bnd_eibd->Write(sideband, address, wdata, size);
          bnd_eibd->Read(sideband, address, rdata, size);
          uint32_t index_16 = (address >> 1) & 0x1;
          memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
          ASSERT_EQ(expect_value, m_data16[index_16]);
      } else if (size == SIZE_32BIT) {
          bnd_eibd->Write(sideband, address, wdata, size);
          bnd_eibd->Read(sideband, address, rdata, size);
          memcpy (&m_data32, rdata, sizeof(m_data32));
          ASSERT_EQ(expect_value, m_data32);
      }
    } else {
          ASSERT_THROW(bnd_eibd->Write(sideband, addr, rdata, size), std::range_error);
          ASSERT_THROW(bnd_eibd->Read(sideband, addr, rdata, size), std::range_error);
    }
}

std::tuple<PhysAddr, uint32_t, ByteSize, ByteSize, int32_t> const RW_ParametersTable_3[] = {
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x07),
std::make_tuple(0xFFF82080, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82081, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x87),
std::make_tuple(0xFFF82081, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82082, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x00),
std::make_tuple(0xFFF82082, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82083, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x83),
std::make_tuple(0xFFF82083, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x8707),
std::make_tuple(0xFFF82080, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF82082, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x8300),
std::make_tuple(0xFFF82082, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_32BIT, SIZE_32BIT, 0x83008707),
std::make_tuple(0xFFF82080, 0x0, SIZE_32BIT, SIZE_32BIT, 0x0),
std::make_tuple(0xFFF82080, 0x0, SIZE_64BIT, SIZE_32BIT, 0x0),
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_64BIT, SIZE_32BIT, 0x0),
};

INSTANTIATE_TEST_CASE_P(TestWithParameters3, TregBND_EIBDTest_3,
                        ::testing::ValuesIn(RW_ParametersTable_3));

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
