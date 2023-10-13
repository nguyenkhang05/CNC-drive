/**
 * @file test_intc_tregintc1_20.cpp
 *
 * @date Created on: 2016
 * @author Author: YVo <y.vo.wg@rvc.renesas.com>
 * @brief UNIT TEST for INTC. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */

#include <string.h>
#include <vector>
#include <memory>
#include <map>
#include <stdexcept>
#include "./config.h"
#include "./fs_controler.h"
#include "./intc1.h"
#include "./exp_info.h"
#include "./fsbus.h"
#include "./proc_element.h"
#include "./forest.h"
#include "mock_sideband.h"
#include "gmock/gmock.h"
#include "./forest_common.h"
#include "./intc_api.h"
#include "./module_vc.h"
#include "./treg.h"
#include "intc1.h"
#include "./intc2.h"

using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;

using ::testing::ExpectationSet;
using ::testing::Expectation;


class TregIntc1Test
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<uint16_t, uint16_t>> {
protected:
    TregIntc1Test() {}
  virtual ~TregIntc1Test() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);

    m_trfs = new TRegIntc1 (pe_id, (CoreType)CORE_G4MH, intc1);

    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete intc1;
                            delete mock_fsbus;
                            delete mock_pe;
                            delete m_trfs;}

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;
  TRegIntc1* m_trfs;

  static const PhysAddr DBMK_OFFSET  = 0x000000080UL;
  ByteSize reg_size = SIZE_16BIT;
  ByteSize min_access_size = SIZE_8BIT;
  PhysAddr base_addr = 0xFFFC0000 + DBMK_OFFSET;
  uint8_t data[8] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

private:
};

TEST_F(TregIntc1Test, CheckTrEICAddr) {
    for (uint32_t ch=0; ch<TRegIntc1::N_EIC; ch++){
        PhysAddr addr;
        addr = m_trfs->TrEICAddr(ch);
        ASSERT_EQ(addr, 0x0 + ch*2);
    }
}

TEST_F(TregIntc1Test, CheckTrEIBDAddr) {
    for (uint32_t ch=0; ch<TRegIntc1::N_EIBD; ch++){
        PhysAddr addr;
        addr = m_trfs->TrEIBDAddr(ch);
        ASSERT_EQ(addr, 0x100 + ch*4);
    }
}

TEST_F(TregIntc1Test, CheckTrFNCAddr) {
    PhysAddr addr;
    addr = m_trfs->TrFNCAddr();
    ASSERT_EQ(addr, 0xA0U);
}

TEST_F(TregIntc1Test, CheckTrFICAddr) {
    for (uint32_t ch=0; ch<TRegIntc1::N_FIC; ch++){
        PhysAddr addr;
        addr = m_trfs->TrFICAddr(ch);
        ASSERT_EQ(addr, 0xC0 + ch*2);
    }
}

TEST_F(TregIntc1Test, CheckTrFNBDAddr) {
    PhysAddr addr;
    addr = m_trfs->TrFNBDAddr();
    ASSERT_EQ(addr, 0x1A0U);
}

TEST_F(TregIntc1Test, CheckTrFIBDAddr) {
    for (uint32_t ch=0; ch<TRegIntc1::N_FIBD; ch++){
        PhysAddr addr;
        addr = m_trfs->TrFIBDAddr(ch);
        ASSERT_EQ(addr, 0x1C0 + ch*4);
    }
}

TEST_F(TregIntc1Test, CheckTrDBMKAddr) {
    PhysAddr addr;
    addr = m_trfs->TrDBMKAddr();
    ASSERT_EQ(addr, 0x80U);
}

TEST_F(TregIntc1Test, CheckTrIHVCFGAddr) {
    PhysAddr addr;
    addr = m_trfs->TrIHVCFGAddr();
    ASSERT_EQ(addr, 0x2F0U);
}

TEST_F(TregIntc1Test, CheckTrEIBGAddr) {
    PhysAddr addr;
    addr = m_trfs->TrEIBGAddr(true);
    ASSERT_EQ(addr, 0x280U);
}

TEST_F(TregIntc1Test, CheckTrFIBGAddr) {
    PhysAddr addr;
    addr = m_trfs->TrFIBGAddr();
    ASSERT_EQ(addr, 0x2C0U);
}

TEST_F(TregIntc1Test, CheckTrEEICAddr) {
    for (uint32_t ch=0; ch<TRegIntc1::N_EEIC; ch++){
        PhysAddr addr;
        addr = m_trfs->TrEEICAddr(ch);
        ASSERT_EQ(addr, 0x0200 + ch*4);
    }
}


TEST_F(TregIntc1Test, CheckInitTRegMap) {
    for (uint32_t i = 0; i<TRegIntc1::N_EIC; i++){
        ASSERT_NE(nullptr, m_trfs->TrEIC(i*2));
    }

    for (uint32_t i = 0; i<TRegIntc1::N_EIBD; i++){
        ASSERT_NE(nullptr, m_trfs->TrEIBD(0x100 + i*4));
    }

    for (uint32_t i = 0; i<1; i++){
        ASSERT_NE(nullptr, m_trfs->TrFNC(0xA0 + i*2));
    }

    for (uint32_t i = 0; i<TRegIntc1::N_FIC; i++){
        ASSERT_NE(nullptr, m_trfs->TrFIC(0xC0 + i*2));
    }

    for (uint32_t i = 0; i<1; i++){
        ASSERT_NE(nullptr, m_trfs->TrFNBD(0x1A0 + i*4));
    }

    for (uint32_t i = 0; i<TRegIntc1::N_FIBD; i++){
        ASSERT_NE(nullptr, m_trfs->TrFIBD(0x1C0 + i*4));
    }

    for (uint32_t i = 0; i<1; i++){
        ASSERT_NE(nullptr, m_trfs->TrDBMK(0x80 + i*4));
    }

    for (uint32_t i = 0; i<1; i++){
        ASSERT_NE(nullptr, m_trfs->TrDBMK(0x2F0 + i*4));
    }
}


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
