/**
 * @file test_intc2_cintc2.cpp
 *
 * @date Created on: Oct 21, 2016
 * @author Author: Can Vy <can.vy.wh@rvc.renesas.com>
 * @brief UNIT TEST for CINTC2.
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

class CIntc2RegAccessTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t>> {
protected:
  CIntc2RegAccessTest() {}
  virtual ~CIntc2RegAccessTest() {}
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
    delete cintc2;
    delete mock_pe;
    delete m_mock_fsbus;
  }

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *m_mock_fsbus;
  PeVec m_pes;
  CIntc2 *cintc2;
  uint32_t pe_id = 0;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

/*!
 * @brief Check INTC2 register read/write operation.
 */
TEST_P(CIntc2RegAccessTest, CheckIntc2RegReadWrite) {
  PhysAddr addr    = std::get<0>(GetParam());
  int32_t  temp    = static_cast<int32_t>(std::get<1>(GetParam()));
  uint8_t  *wdata  = reinterpret_cast<uint8_t *>(&temp);
  uint8_t  rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  ByteSize size = std::get<2>(GetParam());
  ByteSize expect_size = std::get<3>(GetParam());
  uint32_t expect_value = std::get<4>(GetParam());

  cintc2->HardReset();

  {
    SCOPED_TRACE("Write");
    if (size == expect_size) {
      ASSERT_NO_THROW(cintc2->TargetWrite(sideband, addr, wdata, size));
    } else {
      // Expect throw if "Un-guaranteed Size Access".
      ASSERT_THROW(cintc2->TargetWrite(sideband, addr, wdata, size), std::range_error);
    }
  }

  {
    SCOPED_TRACE("Read");
    if (size == expect_size) {
      if (size == SIZE_8BIT) {
        ASSERT_NO_THROW(cintc2->TargetRead(sideband, addr, rdata, size));
        uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (rdata)));
                {
            // output expect value
            // How to get expect values.
            // ctest -R test_float32_minfs -V | grep tuple | sed -e 's/^[0-9]*: //'
            //fprintf(stdout, "std::make_tuple(0x%08x", addr);
            //fprintf(stdout, ", 0x%08x", *wdata);
            //fprintf(stdout, ", %d", size);
            //fprintf(stdout, ", %d", expect_size);
            //fprintf(stdout, ", 0x%08x", expect_value);
            //fprintf(stdout, ", 0x%08x", m_rdata);
            //fprintf(stdout, "),\n");
        }
        ASSERT_EQ(expect_value, m_rdata);
      } else if (size == SIZE_16BIT) {
        ASSERT_NO_THROW(cintc2->TargetRead(sideband, addr, rdata, size));
        uint32_t index_16 = (addr >> 1) & 0x1;
        memcpy (&m_data16[index_16], rdata, sizeof(uint16_t));
                {
            // output expect value
            // How to get expect values.
            // ctest -R test_float32_minfs -V | grep tuple | sed -e 's/^[0-9]*: //'
            //fprintf(stdout, "std::make_tuple(0x%08x", addr);
            //fprintf(stdout, ", 0x%08x", *wdata);
            //fprintf(stdout, ", %d", size);
            //fprintf(stdout, ", %d", expect_size);
            //fprintf(stdout, ", 0x%08x", expect_value);
            //fprintf(stdout, ", 0x%08x", m_data16[index_16]);
            //fprintf(stdout, "),\n");
        }
        ASSERT_EQ(expect_value, m_data16[index_16]);
      } else if (size == SIZE_32BIT) {
        ASSERT_NO_THROW(cintc2->TargetRead(sideband, addr, rdata, size));
        memcpy (&m_data32, rdata, sizeof(m_data32));
                {
            // output expect value
            // How to get expect values.
            // ctest -R test_float32_minfs -V | grep tuple | sed -e 's/^[0-9]*: //'
            //fprintf(stdout, "std::make_tuple(0x%08x", addr);
            //fprintf(stdout, ", 0x%08x", *wdata);
            //fprintf(stdout, ", %d", size);
            //fprintf(stdout, ", %d", expect_size);
            //fprintf(stdout, ", 0x%08x", expect_value);
            //fprintf(stdout, ", 0x%08x", m_data32);
            //fprintf(stdout, "),\n");
        }
        ASSERT_EQ(expect_value, m_data32);
      }
    } else {
      // Expect throw if "Un-guaranteed Size Access".
      ASSERT_THROW(cintc2->TargetRead(sideband, addr, rdata, size), std::range_error);
    }
  }
}
#if _INTC1_20
std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t> const RW_ParametersTable[] = {
std::make_tuple(0xFFF80040, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0xCF),
std::make_tuple(0xFFF80040, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF80041, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x10),
std::make_tuple(0xFFF80041, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF80040, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x10CF),
std::make_tuple(0xFFF80040, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
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
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x07),
std::make_tuple(0xFFF82080, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82081, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x00),
std::make_tuple(0xFFF82081, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82082, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x00),
std::make_tuple(0xFFF82082, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82083, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x83),
std::make_tuple(0xFFF82083, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x0007),
std::make_tuple(0xFFF82080, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF82082, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x8300),
std::make_tuple(0xFFF82082, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_32BIT, SIZE_32BIT, 0x83000007),
std::make_tuple(0xFFF82080, 0x0, SIZE_32BIT, SIZE_32BIT, 0x0),
};
#else
std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t> const RW_ParametersTable[] = {
std::make_tuple(0xFFF80040, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0xCF),
std::make_tuple(0xFFF80040, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF80041, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x10),
std::make_tuple(0xFFF80041, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF80040, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x10CF),
std::make_tuple(0xFFF80040, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
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
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x07),
std::make_tuple(0xFFF82080, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82081, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x00),
std::make_tuple(0xFFF82081, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82082, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x00),
std::make_tuple(0xFFF82082, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82083, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x83),
std::make_tuple(0xFFF82083, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x0007),
std::make_tuple(0xFFF82080, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF82082, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x8300),
std::make_tuple(0xFFF82082, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_32BIT, SIZE_32BIT, 0x83000007),
std::make_tuple(0xFFF82080, 0x0, SIZE_32BIT, SIZE_32BIT, 0x0),
};
#endif
INSTANTIATE_TEST_CASE_P(TestWithParameters1, CIntc2RegAccessTest,
                        ::testing::ValuesIn(RW_ParametersTable));


class CIntc2RegAccessTestDebug
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t>> {
protected:
  CIntc2RegAccessTestDebug() {}
  virtual ~CIntc2RegAccessTestDebug() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    m_mock_fsbus = new MockFsBus();
    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_STANDARD);
    //cintc2->m_eic_eeic = 1;

    EXPECT_CALL((*m_mock_fsbus), RemoveCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*m_mock_fsbus), AddCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), UpdateIntHighPriority(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), IsIntReqInQueue(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), CancelIntByIntc2(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), CancelIntByMask(_)).Times(AtLeast(0));
  }
  virtual void TearDown() {
    delete cintc2;
    delete mock_pe;
    delete m_mock_fsbus;
  }

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *m_mock_fsbus;
  PeVec m_pes;
  CIntc2 *cintc2;
  uint32_t pe_id = 0;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


/*!
 * @brief Check INTC2 register read/write debug operation.
 */
TEST_P(CIntc2RegAccessTestDebug, CheckIntc2RegReadWriteDebug) {
  PhysAddr addr    = std::get<0>(GetParam());
  int32_t  temp    = static_cast<int32_t>(std::get<1>(GetParam()));
  uint8_t  *wdata  = reinterpret_cast<uint8_t *>(&temp);
  uint8_t  rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };

  ByteSize size = std::get<2>(GetParam());
  ByteSize expect_size = std::get<3>(GetParam());
  uint32_t expect_value = std::get<4>(GetParam());

  cintc2->HardReset();

  {
    SCOPED_TRACE("Write");
    if (size == expect_size) {
      ASSERT_NO_THROW(cintc2->TargetWriteDebug(addr, wdata, size));
    } else {
      // Expect throw if "Un-guaranteed Size Access".
      ASSERT_THROW(cintc2->TargetWriteDebug(addr, wdata, size), std::range_error);
    }
  }

  {
    SCOPED_TRACE("Read");
    if (size == expect_size) {
      if (size == SIZE_8BIT) {
        ASSERT_NO_THROW(cintc2->TargetReadDebug(addr, rdata, size));
        uint8_t m_rdata = (*(reinterpret_cast<uint8_t*> (rdata)));
                {
            // output expect value
            // How to get expect values.
            // ctest -R test_float32_minfs -V | grep tuple | sed -e 's/^[0-9]*: //'
            //fprintf(stdout, "std::make_tuple(0x%08x", addr);
            //fprintf(stdout, ", 0x%08x", *wdata);
            //fprintf(stdout, ", %d", size);
            //fprintf(stdout, ", %d", expect_size);
            //fprintf(stdout, ", 0x%08x", expect_value);
            //fprintf(stdout, ", 0x%08x", m_rdata);
            //fprintf(stdout, "),\n");
        }
        ASSERT_EQ(expect_value, m_rdata);
      } else if (size == SIZE_16BIT) {
        ASSERT_NO_THROW(cintc2->TargetReadDebug(addr, rdata, size));
        uint32_t index_16 = (addr >> 1) & 0x1;
        memcpy (&m_data16[index_16], rdata, sizeof(uint16_t));
                {
            // output expect value
            // How to get expect values.
            // ctest -R test_float32_minfs -V | grep tuple | sed -e 's/^[0-9]*: //'
            //fprintf(stdout, "std::make_tuple(0x%08x", addr);
            //fprintf(stdout, ", 0x%08x", *wdata);
            //fprintf(stdout, ", %d", size);
            //fprintf(stdout, ", %d", expect_size);
            //fprintf(stdout, ", 0x%08x", expect_value);
            //fprintf(stdout, ", 0x%08x", m_data16[index_16]);
            //fprintf(stdout, "),\n");
        }
        ASSERT_EQ(expect_value, m_data16[index_16]);
      } else if (size == SIZE_32BIT) {
        ASSERT_NO_THROW(cintc2->TargetReadDebug(addr, rdata, size));
        memcpy (&m_data32, rdata, sizeof(m_data32));
                {
            // output expect value
            // How to get expect values.
            // ctest -R test_float32_minfs -V | grep tuple | sed -e 's/^[0-9]*: //'
            //fprintf(stdout, "std::make_tuple(0x%08x", addr);
            //fprintf(stdout, ", 0x%08x", *wdata);
            //fprintf(stdout, ", %d", size);
            //fprintf(stdout, ", %d", expect_size);
            //fprintf(stdout, ", 0x%08x", expect_value);
            //fprintf(stdout, ", 0x%08x", m_data32);
            //fprintf(stdout, "),\n");
        }
        ASSERT_EQ(expect_value, m_data32);
      }
    } else {
      // Expect throw if "Un-guaranteed Size Access".
      ASSERT_THROW(cintc2->TargetReadDebug(addr, rdata, size), std::range_error);
    }
  }
}
#if _INTC1_20
std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t> const RW_ParametersTableDebug[] = {
std::make_tuple(0xFFF80040, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0xCF),
std::make_tuple(0xFFF80040, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF80041, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x90),
std::make_tuple(0xFFF80041, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF80040, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x90CF),
std::make_tuple(0xFFF80040, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
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
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x07),
std::make_tuple(0xFFF82080, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82081, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x00),
std::make_tuple(0xFFF82081, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82082, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x00),
std::make_tuple(0xFFF82082, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82083, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x83),
std::make_tuple(0xFFF82083, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x0007),
std::make_tuple(0xFFF82080, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF82082, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x8300),
std::make_tuple(0xFFF82082, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_32BIT, SIZE_32BIT, 0x83000007),
std::make_tuple(0xFFF82080, 0x0, SIZE_32BIT, SIZE_32BIT, 0x0),
};
#else
std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t> const RW_ParametersTableDebug[] = {
std::make_tuple(0xFFF80040, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0xCF),
std::make_tuple(0xFFF80040, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF80041, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x90),
std::make_tuple(0xFFF80041, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF80040, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x90CF),
std::make_tuple(0xFFF80040, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
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
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x07),
std::make_tuple(0xFFF82080, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82081, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x00),
std::make_tuple(0xFFF82081, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82082, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x00),
std::make_tuple(0xFFF82082, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82083, 0xFFFFFFFF, SIZE_8BIT, SIZE_8BIT, 0x83),
std::make_tuple(0xFFF82083, 0x0, SIZE_8BIT, SIZE_8BIT, 0x0),
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x0007),
std::make_tuple(0xFFF82080, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF82082, 0xFFFFFFFF, SIZE_16BIT, SIZE_16BIT, 0x8300),
std::make_tuple(0xFFF82082, 0x0, SIZE_16BIT, SIZE_16BIT, 0x0),
std::make_tuple(0xFFF82080, 0xFFFFFFFF, SIZE_32BIT, SIZE_32BIT, 0x83000007),
std::make_tuple(0xFFF82080, 0x0, SIZE_32BIT, SIZE_32BIT, 0x0),
};
#endif
INSTANTIATE_TEST_CASE_P(TestWithParameters2, CIntc2RegAccessTestDebug,
                        ::testing::ValuesIn(RW_ParametersTableDebug));


class CIntc2TestFunctional
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t>> {
protected:
  CIntc2TestFunctional() {}
  virtual ~CIntc2TestFunctional() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    m_mock_fsbus = new MockFsBus();
    cintc2 = new CIntc2(&m_pes, m_mock_fsbus, INTC2_STANDARD);
    //cintc2->m_eic_eeic = 1;

    EXPECT_CALL((*m_mock_fsbus), RemoveCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*m_mock_fsbus), AddCyclicHandler(cintc2)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), UpdateIntHighPriority(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), IsIntReqInQueue(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), CancelIntByIntc2(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), CancelIntByMask(_)).Times(AtLeast(0));
  }
  virtual void TearDown() {
    delete cintc2;
    delete mock_pe;
    delete m_mock_fsbus;
  }

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *m_mock_fsbus;
  PeVec m_pes;
  CIntc2 *cintc2;
  uint32_t pe_id = 0;
  uint32_t channel = 32;
  uint32_t ill_channel = 2048;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};

/*!
 * @brief Check INTC2 reset operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestReset) {
    SCOPED_TRACE("CIntc2TestReset");
    cintc2->HardReset();
    uint8_t data[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
#if _INTC1_20
    uint16_t eic_init_val = 0x008FU;
#else
    uint16_t eic_init_val = 0x008FU;
#endif
    uint32_t imr_init_val = 0xFFFFFFFFU;
    uint16_t eibd_init_val = 0x0U;
    uint32_t eic_addr = 0xFFF80040U;
    uint32_t imr_addr = 0xFFF81080U;
    uint32_t eibd_addr = 0xFFF82080U;
    cintc2->TargetRead(sideband, eic_addr, data, SIZE_16BIT);
    uint32_t index_16 = (eic_addr >> 1) & 0x1;
    memcpy (&m_data16[index_16], data, sizeof(m_data16));
    ASSERT_EQ(eic_init_val, m_data16[index_16]);

    cintc2->TargetRead(sideband, imr_addr, data, SIZE_32BIT);
    memcpy (&m_data32, data, sizeof(m_data32));
    ASSERT_EQ(imr_init_val, m_data32);

    cintc2->TargetRead(sideband, eibd_addr, data, SIZE_32BIT);
    memcpy (&m_data32, data, sizeof(m_data32));
    ASSERT_EQ(eibd_init_val, m_data32);

}


/*!
 * @brief Check INTC2 ReqIntByPeripheral operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestReqIntByPeripheral_NoError) {
    SCOPED_TRACE("CIntc2TestReqIntByPeripheral_NoError");
    cintc2->HardReset();
    uint32_t addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t data = 0x008F;      /*EIC.EIRF=0*/
    uint8_t  *wdata  = reinterpret_cast<uint8_t *>(&data);
    cintc2->TargetWrite(sideband, addr, wdata, SIZE_16BIT);
    /*Check Interrupt error code
     *     INT_E_OK           = 0,  // no error
     *     INT_E_CHANNEL      = 1,  // illegal channel id
     *     INT_E_NOT_RECEIVED = 2,  // interrupt request to no-response channel
     *     INT_E_SYSERR_CODE  = 3   // illegal syserr code
     */
    uint8_t error_code = cintc2->ReqIntByPeripheral(channel);
    EXPECT_EQ(INT_E_OK, error_code);
}


/*!
 * @brief Check INTC2 ReqIntByPeripheral operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestReqIntByPeripheral_IllChannel) {
    SCOPED_TRACE("CIntc2TestReqIntByPeripheral_IllChannel");
    cintc2->HardReset();
    uint32_t addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t data = 0x008F;      /*EIC.EIRF=0*/
    uint8_t  *wdata  = reinterpret_cast<uint8_t *>(&data);
    cintc2->TargetWrite(sideband, addr, wdata, SIZE_16BIT);
    /*Check Interrupt error code
     *     INT_E_OK           = 0,  // no error
     *     INT_E_CHANNEL      = 1,  // illegal channel id
     *     INT_E_NOT_RECEIVED = 2,  // interrupt request to no-response channel
     *     INT_E_SYSERR_CODE  = 3   // illegal syserr code
     */
    uint8_t error_code = cintc2->ReqIntByPeripheral(ill_channel);
    EXPECT_EQ(INT_E_CHANNEL, error_code);
}


/*!
 * @brief Check INTC2 ReqIntByPeripheral operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestReqIntByPeripheral_NoResChannel) {
    SCOPED_TRACE("CIntc2TestReqIntByPeripheral_NoResChannel");
    cintc2->HardReset();
    uint32_t addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t data = 0x108FU;      /*EIC.EIRF=1*/
    uint8_t  *wdata  = reinterpret_cast<uint8_t *>(&data);
    cintc2->TargetWrite(sideband, addr, wdata, SIZE_16BIT);
    /*Check Interrupt error code
     *     INT_E_OK           = 0,  // no error
     *     INT_E_CHANNEL      = 1,  // illegal channel id
     *     INT_E_NOT_RECEIVED = 2,  // interrupt request to no-response channel
     *     INT_E_SYSERR_CODE  = 3   // illegal syserr code
     */
    uint8_t error_code = cintc2->ReqIntByPeripheral(channel);
    EXPECT_EQ(INT_E_NOT_RECEIVED, error_code);
}


/*!
 * @brief Check INTC2 ReqIntByEIC operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestReqIntByEIC_NoBCAS) {
    SCOPED_TRACE("CIntc2TestReqIntByEIC_NoBCAS");
    cintc2->HardReset();
    uint32_t eic_addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t eic_data = 0x1000U;      /*EIC.EIRF=1*/
    uint8_t  *eic_wdata  = reinterpret_cast<uint8_t *>(&eic_data);
    uint32_t eibd_addr = 0xFFF82080U; /*TRegEIBD*/
    uint32_t eibd_data = 0x00000000;  /*EIBD.CST = 0*/
    uint8_t  *eibd_wdata  = reinterpret_cast<uint8_t *>(&eibd_data);

    cintc2->TargetWrite(sideband, eic_addr, eic_wdata, SIZE_16BIT);
    cintc2->TargetWrite(sideband, eibd_addr, eibd_wdata, SIZE_32BIT);

    EXPECT_EQ(true, !cintc2->m_intc2_queue.empty());
}


/*!
 * @brief Check INTC2 ReqIntByEIC operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestReqIntByEIC_BCAS) {
    SCOPED_TRACE("CIntc2TestReqIntByEIC_BCAS");
    cintc2->HardReset();
    uint32_t eic_addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t eic_data = 0x1000U;      /*EIC.EIRF=1*/
    uint8_t  *eic_wdata  = reinterpret_cast<uint8_t *>(&eic_data);
    uint32_t eibd_addr = 0xFFF82080U; /*TRegEIBD*/
    uint32_t eibd_data = 0x80000000U;  /*EIBD.CST = 1*/
    uint8_t  *eibd_wdata  = reinterpret_cast<uint8_t *>(&eibd_data);

    cintc2->TargetWrite(sideband, eic_addr, eic_wdata, SIZE_16BIT);
    cintc2->TargetWrite(sideband, eibd_addr, eibd_wdata, SIZE_32BIT);

    EXPECT_EQ(true, !cintc2->m_intc2_queue.empty());
    EXPECT_EQ(true, !cintc2->m_intc2_bcas.empty());
}


/*!
 * @brief Check INTC2 ReqIntByEIC operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestReqIntByEIC_DelBCAS) {
    SCOPED_TRACE("CIntc2TestReqIntByEIC_DelBCAS");
    cintc2->HardReset();
    uint32_t eic_addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t eic_en_data = 0x1000U;   /*EIC.EIRF=1*/
    uint8_t  *eic_en_wdata  = reinterpret_cast<uint8_t *>(&eic_en_data);
    uint32_t eibd_addr = 0xFFF82080U; /*TRegEIBD*/
    uint32_t eibd_data = 0x80000000U;  /*EIBD.CST = 1*/
    uint8_t  *eibd_wdata  = reinterpret_cast<uint8_t *>(&eibd_data);

    cintc2->TargetWrite(sideband, eic_addr, eic_en_wdata, SIZE_16BIT);
    cintc2->TargetWrite(sideband, eibd_addr, eibd_wdata, SIZE_32BIT);
    cintc2->DelBcasIntInfo(channel);

    EXPECT_EQ(true, !cintc2->m_intc2_queue.empty());
    EXPECT_EQ(true, cintc2->m_intc2_bcas.empty());
}


/*!
 * @brief Check INTC2 CancelInt operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestCancelInt) {
    SCOPED_TRACE("CIntc2TestCancelInt");
    cintc2->HardReset();
    uint32_t eic_addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t eic_en_data = 0x1000U;   /*EIC.EIRF=1*/
    uint8_t  *eic_en_wdata  = reinterpret_cast<uint8_t *>(&eic_en_data);

    cintc2->TargetWrite(sideband, eic_addr, eic_en_wdata, SIZE_16BIT);

    EXPECT_EQ(true, !cintc2->m_intc2_queue.empty());
    cintc2->CancelInt(channel);
    EXPECT_EQ(true, cintc2->m_intc2_queue.empty());

    cintc2->HardReset();
    EXPECT_CALL((*mock_pe), CancelIntByIntc2(_)).Times(AtLeast(0));
    ASSERT_NO_THROW(cintc2->CancelInt(channel));
}


/*!
 * @brief Check INTC2 CancelIntByPeripheral operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestCancelIntByPeripheral) {
    SCOPED_TRACE("CIntc2TestCancelIntByPeripheral");
    cintc2->HardReset();
    cintc2->ReqIntByPeripheral(channel);
    EXPECT_EQ(true, !cintc2->m_intc2_queue.empty());
    cintc2->CancelIntByPeripheral(channel);
    EXPECT_EQ(true, cintc2->m_intc2_queue.empty());
}


/*!
 * @brief Check INTC2 IsIntLevelDetect operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestIsIntLevelDetect) {
    SCOPED_TRACE("CIntc2TestIsIntLevelDetect");
    cintc2->HardReset();
    uint32_t eic_addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t eic_lvl_data = 0x8000U;   /*EIC.EICT=1*/
    uint16_t eic_edg_data = 0x1000U;   /*EIC.EICT=0*/
    uint8_t  *lvl_wdata  = reinterpret_cast<uint8_t *>(&eic_lvl_data);
    uint8_t  *edg_wdata  = reinterpret_cast<uint8_t *>(&eic_edg_data);
    cintc2->TargetWriteDebug(eic_addr, lvl_wdata, SIZE_16BIT);
    bool ret = cintc2->IsIntLevelDetect(channel);
    EXPECT_EQ(true, ret);
    cintc2->TargetWriteDebug(eic_addr, edg_wdata, SIZE_16BIT);
    ret = cintc2->IsIntLevelDetect(channel);
    EXPECT_EQ(true, !ret);
}


/*!
 * @brief Check INTC2 IsIntReqInQueue operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestIsIntReqInQueue) {
    SCOPED_TRACE("CIntc2TestIsIntReqInQueue");
    cintc2->HardReset();
    uint32_t eic_addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t eic_edg_data = 0x1000U;   /*EIC.EICT=0*/
    uint8_t  *edg_wdata  = reinterpret_cast<uint8_t *>(&eic_edg_data);
    cintc2->TargetWrite(sideband, eic_addr, edg_wdata, SIZE_16BIT);
    bool ret = cintc2->IsIntReqInQueue(channel);
    EXPECT_EQ(true, ret);
    cintc2->HardReset();
    ret = cintc2->IsIntReqInQueue(channel);
    EXPECT_EQ(true, !ret);
}
#if _INTC1_20
TEST_F(CIntc2TestFunctional, CIntc2TestIsIntReqInQueue_b) {
    SCOPED_TRACE("CIntc2TestIsIntReqInQueue");
    cintc2->HardReset();
    uint32_t eic_addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t eic_edg_data = 0x1000U;   /*EIC.EICT=0*/
    uint8_t  *edg_wdata  = reinterpret_cast<uint8_t *>(&eic_edg_data);
    cintc2->TargetWriteDebug(eic_addr, edg_wdata, SIZE_16BIT);
    bool ret = cintc2->IsIntReqInQueue(channel);
    EXPECT_EQ(true, ret);
    cintc2->HardReset();
    ret = cintc2->IsIntReqInQueue(channel);
    EXPECT_EQ(true, !ret);
}
#endif

/*!
 * @brief Check INTC2 DelBcasIntInfo operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestDelBcasIntInfo) {
    SCOPED_TRACE("CIntc2TestDelBcasIntInfo");
    cintc2->HardReset();
    uint32_t eic_addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t eic_data = 0x1000U;      /*EIC.EIRF=1*/
    uint8_t  *eic_wdata  = reinterpret_cast<uint8_t *>(&eic_data);
    uint32_t eibd_addr = 0xFFF82080U; /*TRegEIBD*/
    uint32_t eibd_data = 0x80000000U;  /*EIBD.CST = 1*/
    uint8_t  *eibd_wdata  = reinterpret_cast<uint8_t *>(&eibd_data);

    cintc2->TargetWrite(sideband, eic_addr, eic_wdata, SIZE_16BIT);
    cintc2->TargetWrite(sideband, eibd_addr, eibd_wdata, SIZE_32BIT);

    EXPECT_EQ(true, !cintc2->m_intc2_bcas.empty());
    cintc2->DelBcasIntInfo(channel);
    EXPECT_EQ(true, cintc2->m_intc2_bcas.empty());
}


/*!
 * @brief Check INTC2 CreateEiintReqInfo operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestCreateEiintReqInfo) {
    SCOPED_TRACE("CIntc2TestCreateEiintReqInfo");
    cintc2->HardReset();
    EiintReqInfo* intreq_info = cintc2->CreateEiintReqInfo(channel);
    EXPECT_NE(nullptr, intreq_info);
    delete intreq_info;
}


/*!
 * @brief Check INTC2 GetEICPtr operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestGetEICPtr) {
    SCOPED_TRACE("CIntc2TestGetEICPtr");
    uint16_t data_input = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
    cintc2->HardReset();
    //for (; channel < 2048; channel++){
        cintc2->TargetWrite(sideband, 0xFFF80000 + channel*2, wdata, SIZE_16BIT);
        TRegEIC* eic = cintc2->GetEICPtr(channel);
        ASSERT_EQ(channel*2, eic->GetTRegAddr());
    //}
}


/*!
 * @brief Check INTC2 GetEIBDPtr operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestGetEIBDPtr) {
    SCOPED_TRACE("CIntc2TestGetEIBDPtr");
    uint32_t data_input = 0x00000000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
    cintc2->HardReset();
    //for (; channel < 2048; channel++){
        cintc2->TargetWrite(sideband, 0xFFF82000 + channel*4, wdata, SIZE_32BIT);
        TRegEIBD* eibd = cintc2->GetEIBDPtr(channel);
        ASSERT_EQ((0x2000 + channel*4), eibd->GetTRegAddr());
    //}
}


/*!
 * @brief Check INTC2 GetFNCPtr operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestGetFNCPtr) {
    SCOPED_TRACE("CIntc2TestGetFNCPtr");
    cintc2->HardReset();
    ASSERT_EQ(nullptr, cintc2->GetFNCPtr());
}


/*!
 * @brief Check INTC2 GetFICPtr operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestGetFICPtr) {
    SCOPED_TRACE("CIntc2TestGetFICPtr");
    cintc2->HardReset();
    ASSERT_EQ(nullptr, cintc2->GetFICPtr(channel));
}


/*!
 * @brief Check INTC2 GetDBMKPtr operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestGetDBMKPtr) {
    SCOPED_TRACE("CIntc2TestGetFICPtr");
    cintc2->HardReset();
    ASSERT_NE(nullptr, cintc2->MockGetDBMKPtr());
}


/*!
 * @brief Check INTC2 CancelIntByMask operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestCancelIntByMask) {
    SCOPED_TRACE("CIntc2TestCancelIntByMask");
    cintc2->HardReset();
    cintc2->ReqIntByEIC(channel);
    EXPECT_CALL((*mock_pe), CancelIntByMask(channel)).Times(AtLeast(0));
    cintc2->CancelIntByMask(channel);
    EXPECT_EQ(true, !cintc2->m_intc2_queue.empty());
}


/*!
 * @brief Check INTC2 UpdateHighPriority operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestUpdateHighPriority) {
    SCOPED_TRACE("CIntc2TestUpdateHighPriority");
    uint16_t data_input = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data_input);
    cintc2->HardReset();
    cintc2->TargetWrite(sideband, 0xFFF80040, wdata, 2);

    cintc2->m_intc2_priority[pe_id];
    cintc2->UpdateHighPriority(pe_id);
    ASSERT_EQ(0x20U, cintc2->m_intc2_priority[pe_id]->m_channel);
    ASSERT_EQ(0x00, cintc2->m_intc2_priority[pe_id]->m_priority);
}


/*!
 * @brief Check INTC2 AddBcasIntInfo operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestAddBcasIntInfo) {
    SCOPED_TRACE("CIntc2TestAddBcasIntInfo");
    cintc2->HardReset();
    uint32_t eic_addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t eic_data = 0x1000U;      /*EIC.EIRF=1*/
    uint8_t  *eic_wdata  = reinterpret_cast<uint8_t *>(&eic_data);
    uint32_t eibd_addr = 0xFFF82080U; /*TRegEIBD*/
    uint32_t eibd_data = 0x80000000U;  /*EIBD.CST = 1*/
    uint8_t  *eibd_wdata  = reinterpret_cast<uint8_t *>(&eibd_data);

    cintc2->TargetWrite(sideband, eic_addr, eic_wdata, SIZE_16BIT);
    cintc2->TargetWrite(sideband, eibd_addr, eibd_wdata, SIZE_32BIT);


    cintc2->AddBcasIntInfo (channel);
    EXPECT_EQ(true, !cintc2->m_intc2_bcas.empty());
}


/*!
 * @brief Check INTC2 StartCyclicHandler operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestStartCyclicHandler) {
    SCOPED_TRACE("CIntc2TestStartCyclicHandler");
    cintc2->HardReset();
    ASSERT_NO_THROW(cintc2->StartCyclicHandler());
}


/*!
 * @brief Check INTC2 StopCyclicHandler operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestStopCyclicHandler) {
    SCOPED_TRACE("CIntc2TestStopCyclicHandler");
    cintc2->HardReset();
    ASSERT_NO_THROW(cintc2->StopCyclicHandler());
}


/*!
 * @brief Check INTC2 GetImplType operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestGetImplType) {
    SCOPED_TRACE("CIntc2TestGetImplType");
    cintc2->HardReset();
    EXPECT_EQ(INTC2_STANDARD, cintc2->GetImplType());
}


/*!
 * @brief Check INTC2 GetBcasChannel operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestGetBcasChannel) {
    SCOPED_TRACE("CIntc2TestGetBcasChannel");
    uint32_t bcp = 0x0;    /*Value of EIBD.BCP*/
    uint32_t BCAS_BASE_CHANNEL = 4;
    cintc2->HardReset();
    for (; bcp <= 3; bcp++) {
        EXPECT_EQ((BCAS_BASE_CHANNEL + bcp), cintc2->GetBcasChannel(bcp));
    }
}


/*!
 * @brief Check INTC2 ClearIntReqQueue operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestClearIntReqQueue) {
    SCOPED_TRACE("CIntc2TestClearIntReqQueue");
    cintc2->HardReset();
    uint32_t eic_addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t eic_data = 0x1000U;      /*EIC.EIRF=1*/
    uint8_t  *eic_wdata  = reinterpret_cast<uint8_t *>(&eic_data);
    uint32_t eibd_addr = 0xFFF82080U; /*TRegEIBD*/
    uint32_t eibd_data = 0x00000000;  /*EIBD.CST = 0*/
    uint8_t  *eibd_wdata  = reinterpret_cast<uint8_t *>(&eibd_data);

    /*No BCAS*/
    cintc2->TargetWrite(sideband, eic_addr, eic_wdata, SIZE_16BIT);
    cintc2->TargetWrite(sideband, eibd_addr, eibd_wdata, SIZE_32BIT);

    EXPECT_EQ(true, !cintc2->m_intc2_queue.empty());
    cintc2->ClearIntReqQueue();
    EXPECT_EQ(true, cintc2->m_intc2_queue.empty());

    /*BCAS*/
    cintc2->HardReset();
    cintc2->TargetWrite(sideband, eic_addr, eic_wdata, SIZE_16BIT);
    eibd_data = 0x80000000;  /*EIBD.CST = 1*/
    uint8_t *eibd_wdata_2  = reinterpret_cast<uint8_t *>(&eibd_data);
    cintc2->TargetWrite(sideband, eibd_addr, eibd_wdata_2, SIZE_32BIT);

    EXPECT_EQ(true, !cintc2->m_intc2_queue.empty());
    EXPECT_EQ(true, !cintc2->m_intc2_bcas.empty());
    cintc2->ClearIntReqQueue();
    EXPECT_EQ(true, cintc2->m_intc2_queue.empty());
    EXPECT_EQ(true, cintc2->m_intc2_bcas.empty());
}


/*!
 * @brief Check INTC2 IsIntc2Channel operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestIsIntc2Channel) {
    SCOPED_TRACE("CIntc2TestIsIntc2Channel");
    cintc2->HardReset();
    EXPECT_EQ(true, cintc2->IsIntc2Channel(channel));
    EXPECT_NE(true, cintc2->IsIntc2Channel(ill_channel));
}


/*!
 * @brief Check INTC2 IsValidPeId operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestIsValidPeId) {
    SCOPED_TRACE("CIntc2TestIsValidPeId");
    cintc2->HardReset();
    EXPECT_EQ(true, cintc2->IsValidPeId(pe_id));
    EXPECT_NE(true, cintc2->IsValidPeId(1));
}


/*!
 * @brief Check INTC2 ReqBcasIntToIntc1 operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestReqBcasIntToIntc1) {
    SCOPED_TRACE("CIntc2TestReqBcasIntToIntc1");
    cintc2->HardReset();
    uint32_t eic_addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t eic_data = 0x1000U;      /*EIC.EIRF=1*/
    uint8_t  *eic_wdata  = reinterpret_cast<uint8_t *>(&eic_data);
    uint32_t eibd_addr = 0xFFF82080U; /*TRegEIBD*/
    uint32_t eibd_data = 0x80000000;  /*EIBD.CST = 1*/
    uint8_t  *eibd_wdata  = reinterpret_cast<uint8_t *>(&eibd_data);

    cintc2->TargetWrite(sideband, eic_addr, eic_wdata, SIZE_16BIT);
    cintc2->TargetWrite(sideband, eibd_addr, eibd_wdata, SIZE_32BIT);

    EXPECT_CALL((*mock_pe), ReqIntByPeripheral(_)).Times(1);
    cintc2->ReqBcasIntToIntc1();

    EXPECT_EQ(true, cintc2->m_intc2_bcas.empty());
}


/*!
 * @brief Check INTC2 ReqIntToIntc1 operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestReqIntToIntc1) {
    SCOPED_TRACE("CIntc2TestReqIntToIntc1");
    cintc2->HardReset();
    uint32_t eic_addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t eic_data = 0x1000U;      /*EIC.EIRF=1*/
    uint8_t  *eic_wdata  = reinterpret_cast<uint8_t *>(&eic_data);
    EiintReqInfo* intreq_info = cintc2->CreateEiintReqInfo(channel);

    cintc2->TargetWrite(sideband, eic_addr, eic_wdata, SIZE_16BIT);
    EXPECT_CALL((*mock_pe), ReqIntByIntc2(_, _)).Times(1);
    EXPECT_NO_THROW(cintc2->ReqIntToIntc1(pe_id, channel, intreq_info));
    delete intreq_info;
}


/*!
 * @brief Check INTC2 CancelIntToIntc1 operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestCancelIntToIntc1) {
    SCOPED_TRACE("CIntc2TestCancelIntToIntc1");
    cintc2->HardReset();
    uint32_t eic_addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t eic_data = 0x1000U;      /*EIC.EIRF=1*/
    uint8_t  *eic_wdata  = reinterpret_cast<uint8_t *>(&eic_data);
    EiintReqInfo* intreq_info = cintc2->CreateEiintReqInfo(channel);

    cintc2->TargetWrite(sideband, eic_addr, eic_wdata, SIZE_16BIT);
    EXPECT_CALL((*mock_pe), ReqIntByIntc2(_, _)).Times(1);
    EXPECT_NO_THROW(cintc2->ReqIntToIntc1(pe_id, channel, intreq_info));
    EXPECT_CALL((*mock_pe), CancelIntByIntc2(_)).Times(1);
    EXPECT_NO_THROW(cintc2->CancelIntToIntc1(pe_id, channel));
    delete intreq_info;
}


/*!
 * @brief Check INTC2 UpdateHighPriorityOfIntc1 operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestUpdateHighPriorityOfIntc1) {
    SCOPED_TRACE("CIntc2TestUpdateHighPriorityOfIntc1");
    cintc2->HardReset();
    EXPECT_CALL((*mock_pe), UpdateIntHighPriority(_)).Times(1);
    EXPECT_NO_THROW(cintc2->UpdateHighPriorityOfIntc1(pe_id, channel));
}


/*!
 * @brief Check INTC2 IsIntReqInQueueOfIntc1 operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestIsIntReqInQueueOfIntc1) {
    SCOPED_TRACE("CIntc2TestIsIntReqInQueueOfIntc1");
    cintc2->HardReset();
    EXPECT_NO_THROW(cintc2->IsIntReqInQueueOfIntc1(pe_id, channel));
}


/*!
 * @brief Check INTC2 CyclicHandler operation.
 */
TEST_F(CIntc2TestFunctional, CIntc2TestCyclicHandler) {
    SCOPED_TRACE("CIntc2TestCyclicHandler");
    cintc2->HardReset();
    uint32_t eic_addr = 0xFFF80040U; /*TRegEIC*/
    uint16_t eic_data = 0x1000U;      /*EIC.EIRF=1*/
    uint8_t  *eic_wdata  = reinterpret_cast<uint8_t *>(&eic_data);

    uint32_t eibd_addr = 0xFFF82080U; /*TRegEIBD*/
    uint32_t eibd_data = 0x80000000U;  /*EIBD.CST = 1*/
    uint8_t  *eibd_wdata  = reinterpret_cast<uint8_t *>(&eibd_data);

    EXPECT_CALL((*mock_pe), ReqIntByIntc2(_, _)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), ReqIntByPeripheral(_)).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), CancelIntByIntc2(_)).Times(AtLeast(0));

    cintc2->TargetWrite(sideband, eic_addr, eic_wdata, SIZE_16BIT);
    cintc2->TargetWrite(sideband, eibd_addr, eibd_wdata, SIZE_32BIT);

    EXPECT_EQ(true, !cintc2->m_intc2_queue.empty());
    EXPECT_EQ(true, !cintc2->m_intc2_bcas.empty());
    cintc2->CyclicHandler();
    EXPECT_EQ(true, cintc2->m_intc2_queue.empty());
    EXPECT_EQ(true, cintc2->m_intc2_bcas.empty());
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
