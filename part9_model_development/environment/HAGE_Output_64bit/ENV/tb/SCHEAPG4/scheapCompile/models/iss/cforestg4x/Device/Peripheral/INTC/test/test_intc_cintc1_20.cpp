/**
 * @file test_intc_cintc1_20.cpp
 *
 * @date Created on: 2016
  * @author Author: YVo <y.vo.wg@rvc.renesas.com>
 * @brief UNIT TEST for INTC1. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */

#include <functional>
#include <string.h>
#include <vector>
#include <memory>
#include <map>
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include "gmock/gmock.h"
#include "./config.h"
#include "./mock_fsbus.h"
#include "./exp_info.h"
#include "./proc_element.h"
#include "intc1.h"
#include "intc2.h"
#include "mock_sideband.h"
#include "./mock_proc_element.h"
#include "./mock_fsbus.h"
#include "./forest_common.h"
#include "./intc_api.h"
#include "./module_vc.h"
#include "./treg.h"

using ::testing::_;
using ::testing::Return;
using ::testing::AtLeast;

using ::testing::ExpectationSet;
using ::testing::Expectation;


class Intc1RegAccessTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t >> {
protected:
    Intc1RegAccessTest() {}
  virtual ~Intc1RegAccessTest() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);
    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() {
      delete mock_fsbus;
      delete mock_pe;
      delete intc1;
  };

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


/*!
 * @brief Check Intc1 register read/write operation.
 */
TEST_P(Intc1RegAccessTest, CheckIntc1RegReadWrite) {
  PhysAddr addr = std::get<0>(GetParam());
  int32_t temp = static_cast<int32_t>(std::get<1>(GetParam()));
  uint8_t  rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  //uint16_t EIC_mask = 0x10CF;

  ByteSize size = std::get<2>(GetParam());
  ByteSize expect_size = std::get<3>(GetParam());
  uint32_t expect = std::get<4>(GetParam());

  intc1->HardReset();
  intc1->SetIHVEFromIHVCFG(1);

  if (size != expect_size){
    // Expect throw if "Un-guaranteed Size Access".
    ASSERT_THROW(intc1->TargetWrite(sideband, addr, wdata, size), std::range_error);
  }else{
    {
      SCOPED_TRACE("Write");
      if (size == expect_size) {
        ASSERT_NO_THROW(intc1->TargetWrite(sideband, addr, wdata, size));
      }
    }
      {
      SCOPED_TRACE("Read");
      if (size == expect_size) {
          if (size == SIZE_8BIT){
          ASSERT_NO_THROW(intc1->TargetRead(sideband, addr, rdata, size));
          uint32_t index_8 = addr & 0x3;
          memcpy (&m_data8[index_8], rdata, sizeof(uint8_t));
          ASSERT_EQ(expect, m_data8[index_8]);
        }else if (size == SIZE_16BIT){
          ASSERT_NO_THROW(intc1->TargetRead(sideband, addr, rdata, size));
          uint32_t index_16 = (addr >> 1) & 0x1;
          memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
          ASSERT_EQ(expect, m_data16[index_16]);
        }else if (size == SIZE_32BIT){
          ASSERT_NO_THROW(intc1->TargetRead(sideband, addr, rdata, size));
          memcpy (&m_data32, rdata, sizeof(m_data32));
          ASSERT_EQ(expect, m_data32);
        }
      }
    }
  }
}


std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t> const RW_ParametersTable[] = {
  /* 0000 */ std::make_tuple(0xFFFC0000 /* TRegEIC */, 0xf3f2ffff, SIZE_16BIT, SIZE_16BIT, 0x10ef), //16bit access
  /* 0000 */ std::make_tuple(0xFFFC0000 /* TRegEIC */, 0xf3f2ffff, SIZE_8BIT, SIZE_8BIT, 0xef), //8bit access
  /* 0000 */ std::make_tuple(0xFFFC0000 /* TRegEIC */, 0xf3f2ffff, SIZE_32BIT, SIZE_16BIT, 0x10ef), //16bit access
  /* 0000 */ std::make_tuple(0xFFFC0001 /* TRegEIC */, 0xf3f2ffff, SIZE_8BIT, SIZE_8BIT, 0x10), //8bit access
  /* 0000 */ std::make_tuple(0xFFFC00A0 /* TRegFNC */, 0xf3f2ffff, SIZE_16BIT, SIZE_16BIT, 0x0000), //16bit access
  /* 0000 */ std::make_tuple(0xFFFC00C0 /* TRegFIC */, 0xf3f2ffff, SIZE_16BIT, SIZE_16BIT, 0x0000), //16bit access
  /* 0000 */ std::make_tuple(0xFFFC00F0 /* TRegIMR */, 0x11111111, SIZE_32BIT, SIZE_32BIT, 0x11111111), //32bit access
  /* 0000 */ std::make_tuple(0xFFFC0100 /* TRegEIBD*/, 0xffffffff, SIZE_32BIT, SIZE_32BIT, 0x00008700), //32bit access
  /* 0000 */ std::make_tuple(0xFFFC01A0 /* TRegFNBD*/, 0xffffffff, SIZE_32BIT, SIZE_32BIT, 0x00000000), //32bit access
  /* 0000 */ std::make_tuple(0xFFFC01C0 /* TRegFIBD*/, 0xffffffff, SIZE_32BIT, SIZE_32BIT, 0x00008700), //32bit access
  /* 0000 */ std::make_tuple(0xFFFC0080 /* TRegDBMK*/, 0xffffffff, SIZE_16BIT, SIZE_16BIT, 0x00000000), //16bit access
  /* 0000 */ std::make_tuple(0xFFFC0200 /* TRegEEIC */, 0xffffffff, SIZE_32BIT, SIZE_32BIT, 0x10c0803f), //32bit access
  /* 0000 */ std::make_tuple(0xFFFC0200 /* TRegEEIC */, 0x00000000, SIZE_32BIT, SIZE_32BIT, 0x00000000), //32bit access
  /* 0000 */ std::make_tuple(0xFFFC0280 /* TRegEIBG */, 0xffffffff, SIZE_32BIT, SIZE_32BIT, 0x0000003f), //32bit access
  /* 0000 */ std::make_tuple(0xFFFC0280 /* TRegEIBG */, 0x00000000, SIZE_32BIT, SIZE_32BIT, 0x00000000), //32bit access
  /* 0000 */ std::make_tuple(0xFFFC02C0 /* TRegFIBG */, 0xffffffff, SIZE_32BIT, SIZE_32BIT, 0x0000ffff), //32bit access
  /* 0000 */ std::make_tuple(0xFFFC02C0 /* TRegFIBG */, 0x00000000, SIZE_32BIT, SIZE_32BIT, 0x00000000), //32bit access

  // please write more more more test case.
};

INSTANTIATE_TEST_CASE_P(CheckIntc1RegReadWrite_EIC, Intc1RegAccessTest,
                        ::testing::ValuesIn(RW_ParametersTable));

class Intc1RegAccessTestdebug
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t >> {
protected:
    Intc1RegAccessTestdebug() {}
  virtual ~Intc1RegAccessTestdebug() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);
    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() {
      delete mock_fsbus;
      delete mock_pe;
      delete intc1;
  }

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};
TEST_P(Intc1RegAccessTestdebug, CheckIntc1RegReadWrite_DB) {
  PhysAddr addr = std::get<0>(GetParam());
  int32_t temp = static_cast<int32_t>(std::get<1>(GetParam()));
  uint8_t  rdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  //uint16_t EIC_mask = 0x10CF;

  ByteSize size = std::get<2>(GetParam());
  ByteSize expect_size = std::get<3>(GetParam());
  uint32_t expect = std::get<4>(GetParam());

  intc1->HardReset();


  if (size != expect_size){
    // Expect throw if "Un-guaranteed Size Access".o
      ASSERT_THROW(intc1->TargetWriteDebug(addr, wdata, size), std::range_error);
  }else{
    {
      SCOPED_TRACE("Write");
      if (size == expect_size) {
          ASSERT_NO_THROW(intc1->TargetWriteDebug(addr, wdata, size));
      }
    }
      {
      SCOPED_TRACE("Read");
      if (size == expect_size) {
          if (size == SIZE_8BIT){
          ASSERT_NO_THROW(intc1->TargetReadDebug(addr, rdata, size));
          uint32_t index_8 = addr & 0x3;
          memcpy (&m_data8[index_8], rdata, sizeof(uint8_t));
          ASSERT_EQ(expect, m_data8[index_8]);
        }else if (size == SIZE_16BIT){
          ASSERT_NO_THROW(intc1->TargetReadDebug(addr, rdata, size));
          uint32_t index_16 = (addr >> 1) & 0x1;
          memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
          ASSERT_EQ(expect, m_data16[index_16]);
        }else if (size == SIZE_32BIT){
          ASSERT_NO_THROW(intc1->TargetReadDebug(addr, rdata, size));
          memcpy (&m_data32, rdata, sizeof(m_data32));
          ASSERT_EQ(expect, m_data32);
        }
      }
    }
  }
}

std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t> const RW_ParametersTable_debug[] = {
  /* 0000 */ std::make_tuple(0xFFFC0000 /* TRegEIC */, 0xf3f2ffff, SIZE_16BIT, SIZE_16BIT, 0x90ef), //16bit access
  /* 0001 */ std::make_tuple(0xFFFC0000 /* TRegEIC */, 0xf3f2ffff, SIZE_8BIT, SIZE_8BIT, 0xef), //8bit access
  /* 0002 */ std::make_tuple(0xFFFC0000 /* TRegEIC */, 0xf3f2ffff, SIZE_32BIT, SIZE_16BIT, 0x90ef), //16bit access
  /* 0003 */ std::make_tuple(0xFFFC0001 /* TRegEIC */, 0xf3f2ffff, SIZE_8BIT, SIZE_8BIT, 0x90), //8bit access
  /* 0004 */ std::make_tuple(0xFFFC00A0 /* TRegFNC */, 0xf3f2ffff, SIZE_16BIT, SIZE_16BIT, 0x9000), //16bit access
  /* 0005 */ std::make_tuple(0xFFFC00A0 /* TRegFNC */, 0xf3f21fff, SIZE_16BIT, SIZE_16BIT, 0x1000), //16bit access
  /* 0006 */ std::make_tuple(0xFFFC00C0 /* TRegFIC */, 0xf3f2ffff, SIZE_16BIT, SIZE_16BIT, 0x9020), //16bit access
  /* 0007 */ std::make_tuple(0xFFFC00C0 /* TRegFIC */, 0xf3f21fff, SIZE_16BIT, SIZE_16BIT, 0x1020), //16bit access
  /* 0008 */ std::make_tuple(0xFFFC00F0 /* TRegIMR */, 0x11111111, SIZE_32BIT, SIZE_32BIT, 0x11111111), //32bit access
  /* 0009 */ std::make_tuple(0xFFFC0100 /* TRegEIBD*/, 0xffffffff, SIZE_32BIT, SIZE_32BIT, 0x00008700), //32bit access
  /* 0010 */ std::make_tuple(0xFFFC01A0 /* TRegFNBD*/, 0xffffffff, SIZE_32BIT, SIZE_32BIT, 0x00000000), //32bit access
  /* 0011 */ std::make_tuple(0xFFFC01C0 /* TRegFIBD*/, 0xffffffff, SIZE_32BIT, SIZE_32BIT, 0x00008700), //32bit access
  /* 0012 */ std::make_tuple(0xFFFC0080 /* TRegDBMK*/, 0xffffffff, SIZE_16BIT, SIZE_16BIT, 0x00000007), //16bit access
  /* 0013 */ std::make_tuple(0xFFFC0200 /* TRegEEIC */, 0xffffffff, SIZE_32BIT, SIZE_32BIT, 0x90c0803f), //32bit access
  /* 0014 */ std::make_tuple(0xFFFC0200 /* TRegEEIC */, 0x00000000, SIZE_32BIT, SIZE_32BIT, 0x00000000), //32bit access
  /* 0015 */ std::make_tuple(0xFFFC0280 /* TRegEIBG */, 0xffffffff, SIZE_32BIT, SIZE_32BIT, 0x0000003f), //32bit access
  /* 0016 */ std::make_tuple(0xFFFC0280 /* TRegEIBG */, 0x00000000, SIZE_32BIT, SIZE_32BIT, 0x00000000), //32bit access
  /* 0017 */ std::make_tuple(0xFFFC02C0 /* TRegFIBG */, 0xffffffff, SIZE_32BIT, SIZE_32BIT, 0x0000ffff), //32bit access
  /* 0018 */ std::make_tuple(0xFFFC02C0 /* TRegFIBG */, 0x00000000, SIZE_32BIT, SIZE_32BIT, 0x00000000), //32bit access


  // please write more more more test case.
};

INSTANTIATE_TEST_CASE_P(CheckIntc1RegReadWrite_EIC_debug, Intc1RegAccessTestdebug,
                        ::testing::ValuesIn(RW_ParametersTable_debug));

class Intc1Test_Function
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t >> {
protected:
    Intc1Test_Function() {}
  virtual ~Intc1Test_Function() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);
    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() {
      delete mock_fsbus;
      delete mock_pe;
      delete intc1;
  }

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;
  uint32_t eeic_addr = 0xFFFC0200;
  uint32_t eic_addr = 0xFFFC0000;
  uint32_t fnc_addr = 0xFFFC00A0;
  uint32_t fic_addr = 0xFFFC00C0;
  uint32_t dbmk_addr = 0xFFFC0080;

private:
};

TEST_F(Intc1Test_Function, CheckInitPriorityInfo) {
    ASSERT_NO_THROW(intc1->InitPriorityInfo());
    ASSERT_EQ(true, !intc1->m_intc1_priority.empty());
    ASSERT_NE(nullptr, intc1->m_intc1_priority[NC]);
}

TEST_F(Intc1Test_Function, CheckClearIntReqQueue) {
    ASSERT_NO_THROW(intc1->ClearIntReqQueue());
    ASSERT_EQ (true, intc1->m_intc1_queue.empty());
}

TEST_F(Intc1Test_Function, CheckStopCyclicHandler) {
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(1);
    ASSERT_NO_THROW(intc1->StopCyclicHandler(intc1));
}

TEST_F(Intc1Test_Function, CancelEiintByMask){
    typedef std::map<uint32_t, IntReqInfoBase*> IntReqMap;
    uint32_t channel = 0;
    EiintReqInfo* intreq_info = intc1->CreateEiintReqInfo (channel);

    intc1->m_intc1_queue.insert(IntReqMap::value_type (channel, intreq_info));
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    ASSERT_NO_THROW(intc1->CancelEiintByMask(channel));
}

TEST_F(Intc1Test_Function, ReqIntByPeripheralTest1){
    uint32_t channel = 0;
    intc1->HardReset();
    //intc1->m_eic_eeic =1;
    ASSERT_EQ (0x0, intc1->ReqIntByPeripheral(channel));
}

TEST_F(Intc1Test_Function, ReqIntByPeripheralTest2){
    uint32_t channel = 0;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    intc1->TargetWrite(sideband, eic_addr, wdata, 2);
    ASSERT_EQ (0x2, intc1->ReqIntByPeripheral(channel));
}

TEST_F(Intc1Test_Function, ReqIntByPeripheralTest3){
    uint32_t channel = 33;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    intc1->TargetWrite(sideband, eic_addr, wdata, 2);
    ASSERT_EQ (0x1, intc1->ReqIntByPeripheral(channel));
}

TEST_F(Intc1Test_Function, ReqIntByFenmiTest1){
    intc1->HardReset();
    ASSERT_EQ (0x0, intc1->ReqIntByFenmi());
}

TEST_F(Intc1Test_Function, ReqIntByFenmiTest2){
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    sideband.SetGM(0);
    sideband.SetDM(1);
    intc1->TargetWrite(sideband, fnc_addr, wdata, 2);
    ASSERT_EQ (0x2, intc1->ReqIntByFenmi());
}

TEST_F(Intc1Test_Function, ReqIntByFenmiTest2bis){
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    sideband.SetGM(0);
    sideband.SetDM(0);
    intc1->TargetWrite(sideband, fnc_addr, wdata, 2);
    ASSERT_EQ (0x0, intc1->ReqIntByFenmi());
}

TEST_F(Intc1Test_Function, ReqIntByFenmiTest3){
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    sideband.SetUM(1);
    intc1->TargetWrite(sideband, fnc_addr, wdata, 2);
    ASSERT_EQ (0x0, intc1->ReqIntByFenmi());
}

TEST_F(Intc1Test_Function, ReqIntByFeintTest1){
    uint32_t channel = 0;
    intc1->HardReset();
    ASSERT_EQ (0x0, intc1->ReqIntByFeint(channel));
}


TEST_F(Intc1Test_Function, ReqIntByFeintTest2){
    uint32_t channel = 0;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    intc1->SetIHVEFromIHVCFG(1);
    sideband.SetGM(0);
    sideband.SetUM(1);
    sideband.SetGPID(1);
    intc1->TargetWrite(sideband, fic_addr, wdata, 2);
    ASSERT_EQ (0x0, intc1->ReqIntByFeint(channel));
}

TEST_F(Intc1Test_Function, ReqIntByFeintTest3){
    uint32_t channel = 0;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    sideband.SetGM(0);
    sideband.SetDM(1);
    intc1->TargetWrite(sideband, fic_addr, wdata, 2);
    ASSERT_EQ (0x2, intc1->ReqIntByFeint(channel));
}

TEST_F(Intc1Test_Function, ReqIntByFeintTest3bis){
    uint32_t channel = 0;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    sideband.SetGM(0);
    sideband.SetDM(0);
    intc1->TargetWrite(sideband, fic_addr, wdata, 2);
    ASSERT_EQ (0x0, intc1->ReqIntByFeint(channel));
}


TEST_F(Intc1Test_Function, ReqIntByFeintTest4){
    uint32_t channel = 1;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();

    sideband.SetGM(0);
    sideband.SetDM(1);
    intc1->TargetWrite(sideband, fic_addr + 2, wdata, 2);
    ASSERT_EQ (0x2, intc1->ReqIntByFeint(channel));
}


TEST_F(Intc1Test_Function, StartCyclicHandler){
    intc1->HardReset();
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(1);
    ASSERT_NO_THROW(intc1->StartCyclicHandler());
}

TEST_F(Intc1Test_Function, CancelFeintToCpu){
    intc1->HardReset();
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    ASSERT_NO_THROW(intc1->CancelFeintToCpu());
}

TEST_F(Intc1Test_Function, CancelFenmiToCpu){
    intc1->HardReset();
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    ASSERT_NO_THROW(intc1->CancelFenmiToCpu());
}

TEST_F(Intc1Test_Function, CancelIntToCpu){
    intc1->HardReset();
    uint32_t channel = 0;
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    ASSERT_NO_THROW(intc1->CancelIntToCpu(channel));
}

TEST_F(Intc1Test_Function, ReqInt){
    intc1->HardReset();
    uint32_t channel = 0;
    EiintReqInfo* intreq_info = intc1->CreateEiintReqInfo (channel);
    ASSERT_NO_THROW(intc1->ReqInt(channel, intreq_info));
    ASSERT_EQ (true, !intc1->m_intc1_queue.empty());
}

TEST_F(Intc1Test_Function, ReqIntByIntc2){
    uint32_t channel = 0;
    intc1->HardReset();
    EiintReqInfo* intreq_info = intc1->CreateEiintReqInfo (channel);
    ASSERT_NO_THROW(intc1->ReqIntByIntc2(channel, intreq_info));
    ASSERT_EQ (true, !intc1->m_intc1_queue.empty());
}

TEST_F(Intc1Test_Function, ResIntFromCpuTest1){
    uint32_t channel = 0;
    intc1->HardReset();
    ASSERT_NO_THROW(intc1->ResIntFromCpu(channel, intc1));
    ASSERT_EQ(0x0U, intc1->GetEICPtr(channel)->GetEIRF());
}

TEST_F(Intc1Test_Function, ResIntFromCpuTest2){
    uint32_t channel = 0;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    intc1->TargetWrite(sideband, eic_addr, wdata, 2);
    ASSERT_NO_THROW(intc1->ResIntFromCpu(channel, intc1));
    ASSERT_EQ(0x0U, intc1->GetEICPtr(channel)->GetEIRF());
}


TEST_F(Intc1Test_Function, ResFenmiFromCpuTest1){
    intc1->HardReset();
    ASSERT_NO_THROW(intc1->ResFenmiFromCpu(intc1));
    ASSERT_EQ(0x0U, intc1->GetFNCPtr()->GetFNRF());
}

TEST_F(Intc1Test_Function, ResFenmiFromCpuTest2){
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    intc1->TargetWrite(sideband, fnc_addr, wdata, 2);
    ASSERT_NO_THROW(intc1->ResFenmiFromCpu(intc1));
    ASSERT_EQ(0x0U, intc1->GetFNCPtr()->GetFNRF());
}


TEST_F(Intc1Test_Function, ResFeintFromCpuTest1){
    uint32_t channel = 0;
    intc1->HardReset();
    ASSERT_NO_THROW(intc1->ResFeintFromCpu(channel, intc1));
    ASSERT_EQ(0x0U, intc1->GetFICPtr(channel)->GetFIRF());
}

TEST_F(Intc1Test_Function, ResFeintFromCpuTest2){
    uint32_t channel = 0;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    intc1->TargetWrite(sideband, fic_addr, wdata, 2);
    ASSERT_NO_THROW(intc1->ResFeintFromCpu(channel, intc1));
    ASSERT_EQ(0x0U, intc1->GetFICPtr(channel)->GetFIRF());
}

TEST_F(Intc1Test_Function, UpdateHighPriority){
    uint32_t data = 0x1004;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    intc1->InitPriorityInfo();
    intc1->m_intc1_queue.clear();

    intc1->TargetWrite(sideband, eic_addr, wdata, 2);
    intc1->CyclicHandler();

    ASSERT_EQ(0x0U, intc1->m_intc1_priority[-1]->m_channel_eiint);
    ASSERT_EQ(0x4, intc1->m_intc1_priority[-1]->m_priority_eiint);

    uint32_t data_2 = 0x1008;
    uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_2);
    intc1->TargetWrite(sideband, 0xFFFC0002, wdata_2, 2);
    intc1->CyclicHandler();
    ASSERT_EQ(0x0U, intc1->m_intc1_priority[-1]->m_channel_eiint);
    ASSERT_EQ(0x4, intc1->m_intc1_priority[-1]->m_priority_eiint);

    uint32_t data_3 = 0x1006;
    uint8_t *wdata_3 = reinterpret_cast<uint8_t *>(&data_3);
    intc1->TargetWrite(sideband, 0xFFFC0004, wdata_3, 2);
    intc1->CyclicHandler();

    ASSERT_EQ(0x0U, intc1->m_intc1_priority[-1]->m_channel_eiint);
    ASSERT_EQ(0x4, intc1->m_intc1_priority[-1]->m_priority_eiint);

    uint32_t data_4 = 0x1000;
    uint8_t *wdata_4 = reinterpret_cast<uint8_t *>(&data_4);
    intc1->TargetWrite(sideband, 0xFFFC0006, wdata_4, 2);
    intc1->CyclicHandler();

    ASSERT_EQ(0x3U, intc1->m_intc1_priority[-1]->m_channel_eiint);
    ASSERT_EQ(0x0, intc1->m_intc1_priority[-1]->m_priority_eiint);
}

TEST_F(Intc1Test_Function, CreateEiintReqInfo){
    uint32_t channel = 0;
    intc1->HardReset();
    EiintReqInfo* intreq_info = intc1->CreateEiintReqInfo(channel);
    ASSERT_NE(nullptr, intreq_info);
    delete intreq_info;
}

TEST_F(Intc1Test_Function, CreateFenmiReqInfo){
    intc1->HardReset();
    FenmiReqInfo* intreq_info = intc1->CreateFenmiReqInfo();
    ASSERT_NE(nullptr, intreq_info);
    delete intreq_info;
}

TEST_F(Intc1Test_Function, CreateFeintReqInfo){
    uint32_t channel = 0;
    intc1->HardReset();
    FeintReqInfo* intreq_info = intc1->CreateFeintReqInfo(channel);
    ASSERT_NE(nullptr, intreq_info);
    delete intreq_info;
}


TEST_F(Intc1Test_Function, CancelInt){
    uint32_t channel = 0;
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(2);
    intc1->HardReset();
    intc1->ReqIntByEIC(channel);
    intc1->CancelInt(channel);
    ASSERT_EQ(true, intc1->m_intc1_queue.empty());
}

TEST_F(Intc1Test_Function, CancelFenmiByFNRF){
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(2);
    intc1->HardReset();
    intc1->ReqFenmiByFNRF();
    intc1->CancelFenmiByFNRF();
    ASSERT_EQ(true, intc1->m_intc1_queue.empty());
}

TEST_F(Intc1Test_Function, CancelFeintByFIRFTest1){
    uint32_t channel = 0;
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(2);
    intc1->HardReset();
    intc1->ReqFeintByFIRF(channel);
    intc1->CancelFeintByFIRF(channel);
    ASSERT_EQ(true, intc1->m_intc1_queue.empty());
}

TEST_F(Intc1Test_Function, CancelFeintByFIRFTest2){
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(2);
    uint32_t channel = 0;
    bool bindmode = 0;
    uint32_t gpid = 0;
    uint32_t bge = 0;

    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(2);
    intc1->HardReset();
    intc1->ReqFeintByFIRF(channel);

    void (*resfunc)(uint32_t channel, CIntc1* intc1);
    resfunc = &(CIntc1::ResFeintByDummy);
    intc1->ReqFeintToCpu (channel, bindmode, gpid, bge, resfunc);

    intc1->CancelFeintByFIRF(channel);
    ASSERT_EQ(true, intc1->m_intc1_queue.empty());
}


TEST_F(Intc1Test_Function, CancelIntByMask){
    uint32_t channel = 0;
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    intc1->HardReset();
    intc1->ReqIntByEIC(channel);
    intc1->CancelIntByMask(channel);
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}

TEST_F(Intc1Test_Function, CancelFenmiByMask){
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    intc1->HardReset();
    intc1->ReqFenmiByFNRF();
    intc1->CancelFenmiByMask();
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}

TEST_F(Intc1Test_Function, CancelFeintByMaskTest1){
    uint32_t channel = 0;
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(0);
    intc1->HardReset();
    intc1->ReqFeintByFIRF(channel);
    intc1->CancelFeintByMask(channel);
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}

TEST_F(Intc1Test_Function, CancelFeintByMaskTest2){
    uint32_t channel = 0;
    bool bindmode = 0;
    uint32_t gpid = 0;
    uint32_t bge = 0;
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(2);
    intc1->HardReset();
    intc1->ReqFeintByFIRF(channel);

    void (*resfunc)(uint32_t channel, CIntc1* intc1);
    resfunc = &(CIntc1::ResFeintByDummy);
    intc1->ReqFeintToCpu (channel, bindmode, gpid, bge, resfunc);

    intc1->CancelFeintByMask(channel);
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}


TEST_F(Intc1Test_Function, ReqFeintToCpuTest1){     //cpu mode =0, bindmode = 0 --> feint
    uint32_t channel = 0;
    bool bindmode = 0;
    uint32_t gpid = 0;
    uint32_t bge = 0;

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    intc1->HardReset();
    intc1->ReqFeintByFIRF(channel);
    intc1->m_is_cpu_gm = 0;        //CPU in host mode

    void (*resfunc)(uint32_t channel, CIntc1* intc1);
    resfunc = &(CIntc1::ResFeintByDummy);
    intc1->ReqFeintToCpu (channel, bindmode, gpid, bge, resfunc);
}

TEST_F(Intc1Test_Function, ReqFeintToCpuTest2){    //cpu mode =0, bind mode =1 --> no feint
    uint32_t channel = 0;
    bool bindmode = 1;
    uint32_t gpid = 0;
    uint32_t bge = 0;

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(0);
    intc1->HardReset();
    intc1->ReqFeintByFIRF(channel);
    intc1->m_is_cpu_gm = 0;        //CPU in host mode

    void (*resfunc)(uint32_t channel, CIntc1* intc1);
    resfunc = &(CIntc1::ResFeintByDummy);
    intc1->ReqFeintToCpu (channel, bindmode, gpid, bge, resfunc);
}


TEST_F(Intc1Test_Function, ReqFeintToCpuTest3){    //cpu mode =1, bind mode = 0 --> feint
    uint32_t channel = 0;
    bool bindmode = 0;
    uint32_t gpid = 0;
    uint32_t bge = 0;

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    intc1->HardReset();
    intc1->ReqFeintByFIRF(channel);
    intc1->m_is_cpu_gm = 1;        //CPU in guest mode

    void (*resfunc)(uint32_t channel, CIntc1* intc1);
    resfunc = &(CIntc1::ResFeintByDummy);
    intc1->ReqFeintToCpu (channel, bindmode, gpid, bge, resfunc);
}

TEST_F(Intc1Test_Function, ReqFeintToCpuTest4){    //cpu mode =1, bind mode = 1, gpid == cpu_gpid  --> feint (gmfeint)
    uint32_t channel = 0;
    bool bindmode = 1;
    uint32_t gpid = 0;
    uint32_t bge = 0;

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    intc1->HardReset();
    intc1->ReqFeintByFIRF(channel);
    intc1->m_is_cpu_gm = 1;        //CPU in guest mode
    intc1->m_cpu_gpid = 0;        //CPU GPID

    void (*resfunc)(uint32_t channel, CIntc1* intc1);
    resfunc = &(CIntc1::ResFeintByDummy);
    intc1->ReqFeintToCpu (channel, bindmode, gpid, bge, resfunc);
}

TEST_F(Intc1Test_Function, ReqFeintToCpuTest5){    //cpu mode =1, bind mode = 1, gpid != cpu_gpid, bge = 0  --> no feint
    uint32_t channel = 0;
    bool bindmode = 1;
    uint32_t gpid = 0;
    uint32_t bge = 0;

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(0);
    intc1->HardReset();
    intc1->ReqFeintByFIRF(channel);
    intc1->m_is_cpu_gm = 1;        //CPU in guest mode
    intc1->m_cpu_gpid = 1;        //CPU GPID

    void (*resfunc)(uint32_t channel, CIntc1* intc1);
    resfunc = &(CIntc1::ResFeintByDummy);
    intc1->ReqFeintToCpu (channel, bindmode, gpid, bge, resfunc);
}

TEST_F(Intc1Test_Function, ReqFeintToCpuTest6){    //cpu mode =1, bind mode = 1, gpid != cpu_gpid, bge = 1  --> feint (bgfeint)
    uint32_t channel = 0;
    bool bindmode = 1;
    uint32_t gpid = 0;
    uint32_t bge = 1;

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    intc1->HardReset();
    intc1->ReqFeintByFIRF(channel);
    intc1->m_is_cpu_gm = 1;        //CPU in guest mode
    intc1->m_cpu_gpid = 1;        //CPU GPID

    void (*resfunc)(uint32_t channel, CIntc1* intc1);
    resfunc = &(CIntc1::ResFeintByDummy);
    intc1->ReqFeintToCpu (channel, bindmode, gpid, bge, resfunc);
}


TEST_F(Intc1Test_Function, ReqEiintToCpuTest1){     //cpu mode =0, bindmode = 0 --> eeint
    uint32_t channel = 0;
    uint32_t priority = 0;
    bool eitb = 0;
    bool bindmode = 0;
    uint32_t gpid = 0;
    uint32_t bgpr = 0;

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    intc1->HardReset();
    intc1->ReqIntByEIC(channel);
    intc1->m_is_cpu_gm = 0;        //CPU in host mode

    void (*resfunc)(uint32_t channel, CIntc1* intc1);
    resfunc = &(CIntc1::ResIntByDummy);
    intc1->ReqEiintToCpu (priority, channel, eitb, bindmode, gpid, bgpr, resfunc);
}

TEST_F(Intc1Test_Function, ReqEiintToCpuTest2){     //cpu mode =0, bind mode =1 --> no eeint
    uint32_t channel = 0;
    uint32_t priority = 0;
    bool eitb = 0;
    bool bindmode = 1;
    uint32_t gpid = 0;
    uint32_t bgpr = 0;

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(0);
    intc1->HardReset();
    intc1->ReqIntByEIC(channel);
    intc1->m_is_cpu_gm = 0;        //CPU in host mode

    void (*resfunc)(uint32_t channel, CIntc1* intc1);
    resfunc = &(CIntc1::ResIntByDummy);
    intc1->ReqEiintToCpu (priority, channel, eitb, bindmode, gpid, bgpr, resfunc);
}

TEST_F(Intc1Test_Function, ReqEiintToCpuTest3){     //cpu mode =1, bind mode = 0 --> eeint
    uint32_t channel = 0;
    uint32_t priority = 0;
    bool eitb = 0;
    bool bindmode = 0;
    uint32_t gpid = 0;
    uint32_t bgpr = 0;

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    intc1->HardReset();
    intc1->ReqIntByEIC(channel);
    intc1->m_is_cpu_gm = 1;        //CPU in host mode

    void (*resfunc)(uint32_t channel, CIntc1* intc1);
    resfunc = &(CIntc1::ResIntByDummy);
    intc1->ReqEiintToCpu (priority, channel, eitb, bindmode, gpid, bgpr, resfunc);
}

TEST_F(Intc1Test_Function, ReqEiintToCpuTest4){     //cpu mode =1, bind mode = 1, gpid == cpu_gpid  --> eeint (gmeeint)
    uint32_t channel = 0;
    uint32_t priority = 0;
    bool eitb = 0;
    bool bindmode = 1;
    uint32_t gpid = 0;
    uint32_t bgpr = 0;

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    intc1->HardReset();
    intc1->ReqIntByEIC(channel);
    intc1->m_is_cpu_gm = 1;        //CPU in host mode
    intc1->m_cpu_gpid = 0;        //CPU GPID

    void (*resfunc)(uint32_t channel, CIntc1* intc1);
    resfunc = &(CIntc1::ResIntByDummy);
    intc1->ReqEiintToCpu (priority, channel, eitb, bindmode, gpid, bgpr, resfunc);
}

TEST_F(Intc1Test_Function, ReqEiintToCpuTest5){     //cpu mode =1, bind mode = 1, gpid != cpu_gpid, bgpr < priority   --> no eeint
    uint32_t channel = 0;
    uint32_t priority = 2;
    bool eitb = 0;
    bool bindmode = 1;
    uint32_t gpid = 0;
    uint32_t bgpr = 0;

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(0);
    intc1->HardReset();
    intc1->ReqIntByEIC(channel);
    intc1->m_is_cpu_gm = 1;        //CPU in host mode
    intc1->m_cpu_gpid = 1;        //CPU GPID

    void (*resfunc)(uint32_t channel, CIntc1* intc1);
    resfunc = &(CIntc1::ResIntByDummy);
    intc1->ReqEiintToCpu (priority, channel, eitb, bindmode, gpid, bgpr, resfunc);
}

TEST_F(Intc1Test_Function, ReqEiintToCpuTest6){     //cpu mode =1, bind mode = 1, gpid != cpu_gpid, bgpr >= priority   --> eeint
    uint32_t channel = 0;
    uint32_t priority = 0;
    bool eitb = 0;
    bool bindmode = 1;
    uint32_t gpid = 0;
    uint32_t bgpr = 5;

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    intc1->HardReset();
    intc1->ReqIntByEIC(channel);
    intc1->m_is_cpu_gm = 1;        //CPU in host mode
    intc1->m_cpu_gpid = 1;        //CPU GPID

    void (*resfunc)(uint32_t channel, CIntc1* intc1);
    resfunc = &(CIntc1::ResIntByDummy);
    intc1->ReqEiintToCpu (priority, channel, eitb, bindmode, gpid, bgpr, resfunc);
}



TEST_F(Intc1Test_Function, GetEICPtr){
    uint32_t channel;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    for (channel = 0; channel < 32; channel++){
        intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, 2);
        TRegEIC* eic = intc1->GetEICPtr(channel);
        ASSERT_EQ(channel*2, eic->GetTRegAddr());
    }
}

TEST_F(Intc1Test_Function, GetEEICPtr){
    uint32_t channel;
    uint32_t data = 0x10000000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    for (channel = 0; channel < 32; channel++){
        intc1->TargetWrite(sideband, eic_addr + 0x0200 + channel*4, wdata, 4);
        TRegEEIC* eeic = intc1->GetEEICPtr(channel);
        ASSERT_EQ(0x0200 + channel*4, eeic->GetTRegAddr());
    }
}


TEST_F(Intc1Test_Function, GetFNCPtr){
    uint32_t channel;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    for (channel = 0; channel < 1; channel++){
        intc1->TargetWrite(sideband, eic_addr + 0xA0 + channel*2, wdata, 2);
        TRegFNC* fnc = intc1->GetFNCPtr();
        ASSERT_EQ(0xA0 + channel*2, fnc->GetTRegAddr());
    }
}

TEST_F(Intc1Test_Function, GetFICPtr){
    uint32_t channel;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    for (channel = 0; channel < 16; channel++){
        intc1->TargetWrite(sideband, eic_addr + 0xC0 + channel*2, wdata, 2);
        TRegFIC* fic = intc1->GetFICPtr(channel);
        ASSERT_EQ(0xC0 + channel*2, fic->GetTRegAddr());
    }
}

TEST_F(Intc1Test_Function, GetEIBDPtr){
     uint32_t channel;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
       for (channel = 0; channel < 32; channel++){
           intc1->TargetWrite(sideband, eic_addr + 0x100 + channel*4, wdata, 2);
           TRegEIBD* eibd = intc1->GetEIBDPtr(channel);
           ASSERT_EQ(0x100 + channel*4, eibd->GetTRegAddr());
       }
}

TEST_F(Intc1Test_Function, GetFIBDPtr){
     uint32_t channel;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
       for (channel = 0; channel < 16; channel++){
           intc1->TargetWrite(sideband, eic_addr + 0x1C0 + channel*4, wdata, 2);
           TRegFIBD* fibd = intc1->GetFIBDPtr(channel);
           ASSERT_EQ(0x1C0 + channel*4, fibd->GetTRegAddr());
       }
}

TEST_F(Intc1Test_Function, GetFNBDPtr){
     uint32_t channel;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
       for (channel = 0; channel < 1; channel++){
           intc1->TargetWrite(sideband, eic_addr + 0x1A0 + channel*4, wdata, 2);
           TRegFNBD* fnbd = intc1->GetFNBDPtr();
           ASSERT_EQ(0x1A0 + channel*4, fnbd->GetTRegAddr());
       }
}

TEST_F(Intc1Test_Function, GetDBMKPtr){
     uint32_t channel;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
       for (channel = 0; channel < 1; channel++){
           intc1->TargetWrite(sideband, eic_addr + 0x80 + channel*2, wdata, 2);
           TRegDBMK* dbmk = intc1->GetDBMKPtr();
           ASSERT_EQ(0x80 + channel*4, dbmk->GetTRegAddr());
       }
}

TEST_F(Intc1Test_Function, GetIHVCFGPtr){
     uint32_t channel;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
       for (channel = 0; channel < 1; channel++){
           intc1->TargetWrite(sideband, eic_addr + 0x80 + channel*2, wdata, 2);
           TRegIHVCFG* ihvcfg = intc1->GetIHVCFGPtr();
           ASSERT_EQ(0x2F0 + channel*4, ihvcfg->GetTRegAddr());
       }
}


TEST_F(Intc1Test_Function, GetEIBGPtr){
     uint32_t channel;
    uint32_t data = 0x10000000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
       for (channel = 0; channel < 1; channel++){
           intc1->TargetWrite(sideband, eic_addr + 0x280 + channel*4, wdata, 4);
           TRegEIBG* eibg = intc1->GetEIBGPtr();
           ASSERT_EQ(0x280 + channel*4, eibg->GetTRegAddr());
       }
}

TEST_F(Intc1Test_Function, GetFIBGPtr){
     uint32_t channel;
    uint32_t data = 0x10000000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
       for (channel = 0; channel < 1; channel++){
           intc1->TargetWrite(sideband, eic_addr + 0x2C0 + channel*4, wdata, 4);
           TRegFIBG* fibg = intc1->GetFIBGPtr();
           ASSERT_EQ(0x2C0 + channel*4, fibg->GetTRegAddr());
       }
}



TEST_F(Intc1Test_Function, ScenarioTest1){
    uint32_t channel = 0;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(3);
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, 2);
    intc1->CyclicHandler();
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}

TEST_F(Intc1Test_Function, ScenarioTest1_e){
    uint32_t channel = 0;
    uint32_t data = 0x10000000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(3);
    intc1->TargetWrite(sideband, eeic_addr + channel*4, wdata, 4);
    intc1->CyclicHandler();
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}


TEST_F(Intc1Test_Function, ScenarioTest2_e){
    uint32_t channel = 0;
    //uint32_t data = 0x10ff;
    uint32_t data = 0x10c0000f;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(0);// not send int req to outside due to EIMK=1
    intc1->TargetWrite(sideband, eeic_addr + channel*4, wdata, 4);
    intc1->CyclicHandler();
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}

TEST_F(Intc1Test_Function, ScenarioTest2){
    uint32_t channel = 0;
    uint32_t data = 0x10ff;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(0);// not send int req to outside due to EIMK=1
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, 2);
    intc1->CyclicHandler();
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}

TEST_F(Intc1Test_Function, ScenarioTest3){
    uint32_t channel = 0;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();

    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, 2);    //EIC
    uint32_t data_2 = 0x0001;
    uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_2);
    sideband.SetUM(1);
    intc1->TargetWrite(sideband, dbmk_addr + channel*2, wdata_2, 2);    //DBMK

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(3);
    intc1->CyclicHandler();
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}

TEST_F(Intc1Test_Function, ScenarioTest3_e){
    uint32_t channel = 0;
    //uint32_t data = 0x1000;
    uint32_t data = 0x10000000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();

    intc1->TargetWrite(sideband, eeic_addr + channel*4, wdata, 4);    //EEIC
    uint32_t data_2 = 0x0001;
    uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_2);
    sideband.SetUM(1);
    intc1->TargetWrite(sideband, dbmk_addr + channel*2, wdata_2, 2);    //DBMK

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(3);
    intc1->CyclicHandler();
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}



TEST_F(Intc1Test_Function, ScenarioTest4){
    int32_t channel = 0;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    sideband.SetGM(0);
    intc1->HardReset();

    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, 2);    //EIC
    sideband.SetDM(1);
    uint32_t data_2 = 0x0001;
    uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_2);
    intc1->TargetWrite(sideband, dbmk_addr + channel*2, wdata_2, 2);    //DBMK

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(0);
    intc1->CyclicHandler();
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}

TEST_F(Intc1Test_Function, ScenarioTest4_e){
    int32_t channel = 0;
    //uint32_t data = 0x1000;
    uint32_t data = 0x10000000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    sideband.SetGM(0);
    intc1->HardReset();

    intc1->TargetWrite(sideband, eeic_addr + channel*4, wdata, 4);    //EIC
    sideband.SetDM(1);
    uint32_t data_2 = 0x0001;
    uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_2);
    intc1->TargetWrite(sideband, dbmk_addr + channel*2, wdata_2, 2);    //DBMK

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(0);
    intc1->CyclicHandler();
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}



TEST_F(Intc1Test_Function, ScenarioTest5){
    uint32_t channel = 0;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);

    sideband.SetGM(0);
    sideband.SetDM(1);
    intc1->HardReset();
    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata, 2);

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(3);
    intc1->CyclicHandler();
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}

TEST_F(Intc1Test_Function, ScenarioTest6){
    uint32_t channel = 0;
    uint32_t data = 0x9000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    sideband.SetDM(1);

    intc1->HardReset();
    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata, 2);

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(3);
    intc1->CyclicHandler();
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}

TEST_F(Intc1Test_Function, ScenarioTest6_b){
    uint32_t channel = 0;
    uint32_t data = 0x9000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);

    intc1->HardReset();
    intc1->TargetWriteDebug(fic_addr + channel*2, wdata, 2);

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(3);
    intc1->CyclicHandler();
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}

TEST_F(Intc1Test_Function, ScenarioTest7){
    uint32_t channel = 0;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    intc1->SetIHVEFromIHVCFG(1);

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(0);
    sideband.SetGM(1);
    sideband.SetUM(0);
    sideband.SetGPID(1);
    sideband.SetDM(1);
    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata, 2);    //FIC
    intc1->CyclicHandler();
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, flag);
}

TEST_F(Intc1Test_Function, ScenarioTest8){
    int32_t channel = 0;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);

    uint32_t data_2 = 0x0002;
    uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_2);
    intc1->TargetWrite(sideband, dbmk_addr + channel*2, wdata_2, 2);    //DBMK

    sideband.SetGM(0);
    sideband.SetDM(1);
    intc1->HardReset();

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(0);// m_req_feint_channel= channel at CyclicHandler, by calling ReqFeintToCpu(), before that m_req_feint_channel != channel
    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata, 2);    //FIC
    sideband.SetDM(1);
    intc1->TargetWrite(sideband, dbmk_addr + channel*2, wdata_2, 2);    //DBMK

    intc1->CyclicHandler();
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}

TEST_F(Intc1Test_Function, ScenarioTest9){
    uint32_t channel = 0;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);

    sideband.SetGM(0);
    sideband.SetDM(1);
    intc1->HardReset();

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    intc1->TargetWrite(sideband, fnc_addr + channel*2, wdata, 2);
    intc1->CyclicHandler();
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}


TEST_F(Intc1Test_Function, ScenarioTest10){
    uint32_t channel = 0;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    sideband.SetUM(1);

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(0);
    intc1->TargetWrite(sideband, fnc_addr + channel*2, wdata, 2);    //FNC
    intc1->CyclicHandler();
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, flag);
}


TEST_F(Intc1Test_Function, ScenarioTest11){
    int32_t channel = 0;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);

    uint32_t data_2 = 0x0004;
    uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_2);
    intc1->TargetWrite(sideband, dbmk_addr + channel*2, wdata_2, 2);    //DBMK

    sideband.SetGM(0);
    sideband.SetDM(1);
    intc1->HardReset();

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(1);
    intc1->TargetWrite(sideband, fnc_addr + channel*2, wdata, 2);    //FNC
    sideband.SetDM(1);
    intc1->TargetWrite(sideband, dbmk_addr + channel*2, wdata_2, 2);    //DBMK

    intc1->CyclicHandler();
    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}

class Intc1Test_Function2
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t >> {
protected:
    Intc1Test_Function2() {}
  virtual ~Intc1Test_Function2() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);
    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() {
      delete mock_fsbus;
      delete mock_pe;
      delete intc1;
  }

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;
  uint32_t eeic_addr = 0xFFFC0200;
  uint32_t eic_addr = 0xFFFC0000;
  uint32_t fnc_addr = 0xFFFC00A0;
  uint32_t fic_addr = 0xFFFC00C0;
  uint32_t dbmk_addr = 0xFFFC0080;

private:
};

TEST_F(Intc1Test_Function2, ScenarioTest12){
    int32_t channel = 4;
    uint32_t data = 0x1005;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);

    sideband.SetGM(0);
    sideband.SetDM(1);
    intc1->HardReset();

    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, 2);    //EIC
    intc1->CyclicHandler();

    ASSERT_EQ(0x4U, intc1->m_intc1_priority[-1]->m_channel_eiint);
    ASSERT_EQ(0x5, intc1->m_intc1_priority[-1]->m_priority_eiint);

    uint32_t data_2 = 0x1000;
    uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_2);
    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata_2, 2);    //FIC
    intc1->CyclicHandler();
    ASSERT_EQ(0x4U, (intc1->m_intc1_priority[-1]->m_channel_feint) - 0x1001);
    ASSERT_EQ(-12, intc1->m_intc1_priority[-1]->m_priority_feint);

    intc1->TargetWrite(sideband, fic_addr , wdata_2, 2);    //FIC
    intc1->CyclicHandler();
    ASSERT_EQ(0x0U, (intc1->m_intc1_priority[-1]->m_channel_feint) - 0x1001);
    ASSERT_EQ(-16, intc1->m_intc1_priority[-1]->m_priority_feint);

    intc1->TargetWrite(sideband, fnc_addr , wdata_2, 2);    //FNC
    intc1->CyclicHandler();
    ASSERT_EQ(0x0U, (intc1->m_intc1_priority[-1]->m_channel_fenmi) - 0x1000);
    ASSERT_EQ(-17, intc1->m_intc1_priority[-1]->m_priority_fenmi);

    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}

TEST_F(Intc1Test_Function2, ScenarioTest12_e){
    int32_t channel = 4;
    //uint32_t data = 0x1005;
    uint32_t data = 0x10000005;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);

    sideband.SetGM(0);
    sideband.SetDM(1);
    intc1->HardReset();

    intc1->TargetWrite(sideband, eeic_addr + channel*4, wdata, 4);    //EIC
    intc1->CyclicHandler();

    ASSERT_EQ(0x4U, intc1->m_intc1_priority[-1]->m_channel_eiint);
    ASSERT_EQ(0x5, intc1->m_intc1_priority[-1]->m_priority_eiint);

    uint32_t data_2 = 0x1000;
    uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_2);
    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata_2, 2);    //FIC
    intc1->CyclicHandler();
    ASSERT_EQ(0x4U, (intc1->m_intc1_priority[-1]->m_channel_feint) - 0x1001);
    ASSERT_EQ(-12, intc1->m_intc1_priority[-1]->m_priority_feint);

    intc1->TargetWrite(sideband, fic_addr , wdata_2, 2);    //FIC
    intc1->CyclicHandler();
    ASSERT_EQ(0x0U, (intc1->m_intc1_priority[-1]->m_channel_feint) - 0x1001);
    ASSERT_EQ(-16, intc1->m_intc1_priority[-1]->m_priority_feint);

    intc1->TargetWrite(sideband, fnc_addr , wdata_2, 2);    //FNC
    intc1->CyclicHandler();
    ASSERT_EQ(0x0U, (intc1->m_intc1_priority[-1]->m_channel_fenmi) - 0x1000);
    ASSERT_EQ(-17, intc1->m_intc1_priority[-1]->m_priority_fenmi);

    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);
}

TEST_F(Intc1Test_Function2, ScenarioTest13){
    int32_t channel = 4;
    uint32_t data = 0x1005;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);

    uint32_t data_2 = 0x1000;
    uint8_t *wdata_2 = reinterpret_cast<uint8_t *>(&data_2);

    sideband.SetGM(0);
    sideband.SetDM(1);
    intc1->HardReset();

    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, 2);    //EIC
    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata_2, 2);    //FIC
    intc1->TargetWrite(sideband, fic_addr , wdata_2, 2);    //FIC
    intc1->TargetWrite(sideband, fnc_addr , wdata_2, 2);    //FNC
    intc1->CyclicHandler();

    ASSERT_EQ(0x4U, intc1->m_intc1_priority[-1]->m_channel_eiint);
    ASSERT_EQ(0x5, intc1->m_intc1_priority[-1]->m_priority_eiint);

    ASSERT_EQ(0x0U, (intc1->m_intc1_priority[-1]->m_channel_feint) - 0x1001);
    ASSERT_EQ(-16, intc1->m_intc1_priority[-1]->m_priority_feint);

    ASSERT_EQ(0x0U, (intc1->m_intc1_priority[-1]->m_channel_fenmi) - 0x1000);
    ASSERT_EQ(-17, intc1->m_intc1_priority[-1]->m_priority_fenmi);

    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);

}


TEST_F(Intc1Test_Function2, CheckIsIntc1Channel ) {
     for (uint32_t channel = 0; channel <32; channel++){
         ASSERT_EQ (true, intc1->IsIntc1Channel(channel));
     }

     for (uint32_t channel = 32; channel <2048; channel++){
         ASSERT_EQ (true, !intc1->IsIntc1Channel(channel));
     }
}


class Intc1EIC_EEICTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t, PhysAddr, int32_t, ByteSize >> {
protected:
    Intc1EIC_EEICTest() {}
  virtual ~Intc1EIC_EEICTest() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);
    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() {
      delete mock_fsbus;
      delete mock_pe;
      delete intc1;
  };

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;
  union {
      uint32_t m_data32;
      uint16_t m_data16[2];
      uint8_t  m_data8[4];
  };

private:
};


/*!
 * @brief Check Intc1 register read/write operation.
 */
TEST_P(Intc1EIC_EEICTest, CheckIntc1RegReadWrite) {
  PhysAddr addr = std::get<0>(GetParam());
  int32_t temp = static_cast<int32_t>(std::get<1>(GetParam()));
  uint8_t  rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);

  ByteSize size = std::get<2>(GetParam());
  ByteSize expect_size = std::get<3>(GetParam());
  uint32_t expect = std::get<4>(GetParam());

  PhysAddr mirror_addr = std::get<5>(GetParam());
  uint32_t mirror_expect = std::get<6>(GetParam());
  ByteSize mirror_size = std::get<7>(GetParam());

  intc1->HardReset();


  if (size != expect_size){
    // Expect throw if "Un-guaranteed Size Access".
    ASSERT_THROW(intc1->TargetWrite(sideband, addr, wdata, size), std::range_error);
  }else{
    {
      SCOPED_TRACE("Write");
      if (size == expect_size) {
        ASSERT_NO_THROW(intc1->TargetWrite(sideband, addr, wdata, size));
      }
    }
      {
      SCOPED_TRACE("Read");
      if (size == expect_size) {
          if (size == SIZE_8BIT){
          ASSERT_NO_THROW(intc1->TargetRead(sideband, addr, rdata, size));
          uint32_t index_8 = addr & 0x3;
          memcpy (&m_data8[index_8], rdata, sizeof(uint8_t));
          ASSERT_EQ(expect, m_data8[index_8]);
        }else if (size == SIZE_16BIT){
          ASSERT_NO_THROW(intc1->TargetRead(sideband, addr, rdata, size));
          uint32_t index_16 = (addr >> 1) & 0x1;
          memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
          ASSERT_EQ(expect, m_data16[index_16]);
          ASSERT_NO_THROW(intc1->TargetRead(sideband, mirror_addr, rdata, mirror_size));
          memcpy (&m_data32, rdata, sizeof(m_data32));
          ASSERT_EQ(mirror_expect, m_data32);
        }else if (size == SIZE_32BIT){
          ASSERT_NO_THROW(intc1->TargetRead(sideband, addr, rdata, size));
          memcpy (&m_data32, rdata, sizeof(m_data32));
          ASSERT_EQ(expect, m_data32);
          ASSERT_NO_THROW(intc1->TargetRead(sideband, mirror_addr, rdata, mirror_size));
          uint32_t index_16 = (addr >> 1) & 0x1;
          memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
          ASSERT_EQ(mirror_expect, m_data16[index_16]);
        }
      }
    }
  }
}


std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t, PhysAddr, int32_t, ByteSize > const RW_ParametersTable_eic_eeic[] = {
  /* 0000 */ std::make_tuple(0xFFFC0000 /* TRegEIC */, 0xffffffff, SIZE_16BIT, SIZE_16BIT, 0x10ef, 0xFFFC0200, 0x10c0800f, SIZE_32BIT), //16bit access
  /* 0000 */ std::make_tuple(0xFFFC0000 /* TRegEIC */, 0xf3f20000, SIZE_16BIT, SIZE_16BIT, 0x0000, 0xFFFC0200, 0x0, SIZE_32BIT ), //8bit access
  /* 0000 */ std::make_tuple(0xFFFC0200 /* TRegEEIC */, 0xffffffff, SIZE_32BIT, SIZE_32BIT, 0x10c0803f, 0xFFFC0000, 0x10ef, SIZE_16BIT), //32bit access
  /* 0000 */ std::make_tuple(0xFFFC0200 /* TRegEEIC */, 0x00000000, SIZE_32BIT, SIZE_32BIT, 0x00000000, 0xFFFC0000, 0x0, SIZE_16BIT ), //32bit access

  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(CheckIntc1RegReadWrite_EIC, Intc1EIC_EEICTest,
                        ::testing::ValuesIn(RW_ParametersTable_eic_eeic));

class Intc1xIOVFunctionTest
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t >> {
protected:
    Intc1xIOVFunctionTest() {}
  virtual ~Intc1xIOVFunctionTest() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);
    EXPECT_CALL((*mock_pe), GetTcLastId()).Times(AtLeast(0));
    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() {
      delete mock_fsbus;
      delete mock_pe;
      delete intc1;
  }

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;
  uint32_t eeic_addr = 0xFFFC0200;
  uint32_t eic_addr = 0xFFFC0000;
  uint32_t fnc_addr = 0xFFFC00A0;
  uint32_t fic_addr = 0xFFFC00C0;
  uint32_t dbmk_addr = 0xFFFC0080;

  union {
        uint32_t m_data32;
        uint16_t m_data16[2];
        uint8_t  m_data8[4];
  };

private:
};

TEST_F(Intc1xIOVFunctionTest, UpdateOverwriteBitEICTest1){
    uint32_t channel = 0;
    uint32_t data = 0x1080;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    uint8_t  rdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    intc1->HardReset();

    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, 2);

    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);

    intc1->UpdateOverwriteBit_EIC(0);
    intc1->TargetRead(sideband, eic_addr + channel*2, rdata, 2);
    uint32_t index_16 = (eic_addr >> 1) & 0x1;
    memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
    ASSERT_EQ(0x10a0, m_data16[index_16]);
}

TEST_F(Intc1xIOVFunctionTest, UpdateOverwriteBitEICTest2){
    uint32_t channel = 0;
    uint32_t data = 0x1080;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    uint8_t  rdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    intc1->HardReset();

    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, 2);
    TRegEIC *eic = intc1->GetEICPtr(channel);
    eic->SetEICT(1);

    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);

    intc1->UpdateOverwriteBit_EIC(0);
    intc1->TargetRead(sideband, eic_addr + channel*2, rdata, 2);
    uint32_t index_16 = (eic_addr >> 1) & 0x1;
    memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
    ASSERT_EQ(0x9080, m_data16[index_16]);
}


TEST_F(Intc1xIOVFunctionTest, UpdateOverwriteBitFICTest2){
    uint32_t channel = 0;
    uint32_t data = 0x2;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);

    uint32_t data1 = 0x1000;
    uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&data1);

    uint8_t  rdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    intc1->HardReset();
    sideband.SetDM(1);

    intc1->TargetWriteDebug(dbmk_addr + channel*2, wdata, 2);        //FEINT is masked
    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata1, 2);
    TRegFIC *fic = intc1->GetFICPtr(channel);
    fic->SetFICT(1);

    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);

    intc1->UpdateOverwriteBit_FIC(0);
    intc1->TargetRead(sideband, fic_addr + channel*2, rdata, 2);
    uint32_t index_16 = (fic_addr >> 1) & 0x1;
    memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
    ASSERT_EQ(0x9000, m_data16[index_16]);
}

TEST_F(Intc1xIOVFunctionTest, UpdateOverwriteBitFICTest1){
    uint32_t channel = 0;
    uint32_t data = 0x2;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);

    uint32_t data1 = 0x1000;
    uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&data1);

    uint8_t  rdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    intc1->HardReset();
    sideband.SetDM(1);

    intc1->TargetWriteDebug(dbmk_addr + channel*2, wdata, 2);        //FEINT is masked
    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata1, 2);


    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);

    intc1->UpdateOverwriteBit_FIC(0);
    intc1->TargetRead(sideband, fic_addr + channel*2, rdata, 2);
    uint32_t index_16 = (fic_addr >> 1) & 0x1;
    memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
    ASSERT_EQ(0x1020, m_data16[index_16]);
}

TEST_F(Intc1xIOVFunctionTest, EIOVTest1){
    uint32_t channel = 0;
    uint32_t data = 0x1080;            //mask = 1
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    uint8_t  rdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    intc1->HardReset();

    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, 2);
    intc1->CyclicHandler();

    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);

    uint32_t data1 = 0x1000;        //mask = 0
    uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&data1);
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata1, 2);
    intc1->CyclicHandler();

    intc1->TargetRead(sideband, eic_addr + channel*2, rdata, 2);
    uint32_t index_16 = (eic_addr >> 1) & 0x1;
    memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
    ASSERT_EQ(0x1000, m_data16[index_16]);
}

TEST_F(Intc1xIOVFunctionTest, EIOVTest2){
    uint32_t channel = 0;
    uint32_t data = 0x1000;        //mask = 0
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    uint8_t  rdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    intc1->HardReset();

    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, 2);
    intc1->CyclicHandler();

    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);

    uint32_t data1 = 0x1080;        //mask = 1
    uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&data1);
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata1, 2);
    intc1->CyclicHandler();

    intc1->TargetRead(sideband, eic_addr + channel*2, rdata, 2);
    uint32_t index_16 = (eic_addr >> 1) & 0x1;
    memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
    ASSERT_EQ(0x1080, m_data16[index_16]);
}

TEST_F(Intc1xIOVFunctionTest, EIOVTest3){
    uint32_t channel = 0;
    uint32_t data = 0x1080;        //mask = 0, eirf = 1
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, 2);


    uint32_t data1 = 0x0000;        //mask = 0, eirf = 0
    uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&data1);
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata1, 2);

    intc1->CyclicHandler();

    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, flag);

}

//1. channel 0: mask =1, eirf=1, cyclichandler()
//2. channel 0: mask =1, eirf=1, cyclichandler()
//expected: m_intc1_queue is not empty
//expected: m_intc1_pending_queue is not empty
//expected: eic.data = 0x1080

TEST_F(Intc1xIOVFunctionTest, EIOVTest4){
    uint32_t channel = 0;
    uint32_t data = 0x1080;        //mask = 1, eirf = 1
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    uint8_t  rdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    intc1->HardReset();
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, 2);
    intc1->CyclicHandler();

    uint32_t data1 = 0x1080;        //mask = 1, eirf = 1
    uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&data1);
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata1, 2);
    intc1->CyclicHandler();

    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);

    intc1->TargetRead(sideband, eic_addr + channel*2, rdata, 2);
    uint32_t index_16 = (eic_addr >> 1) & 0x1;
    memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
    ASSERT_EQ(0x1080, m_data16[index_16]);
}

TEST_F(Intc1xIOVFunctionTest, EIOVTest5){
    uint32_t channel = 0;
    uint32_t data = 0x1004;        //mask = 0, eirf = 1
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    uint8_t  rdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    intc1->HardReset();
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, 2);
    intc1->CyclicHandler();

    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);

    uint32_t data1 = 0x1000;        //mask = 0, eirf = 1
    uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&data1);
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata1, 2);
    intc1->CyclicHandler();

    intc1->TargetRead(sideband, eic_addr + channel*2, rdata, 2);
    uint32_t index_16 = (eic_addr >> 1) & 0x1;
    memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
    ASSERT_EQ(0x1000, m_data16[index_16]);
}

TEST_F(Intc1xIOVFunctionTest, EIOVTest6){
    uint32_t channel = 0;
    uint32_t data = 0x1080;        //mask = 1, eirf = 1
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    uint8_t  rdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    intc1->HardReset();
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, 2);
    intc1->CyclicHandler();

    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);

    intc1->ReqIntByPeripheral(0);
    intc1->CyclicHandler();

    intc1->TargetRead(sideband, eic_addr + channel*2, rdata, 2);
    uint32_t index_16 = (eic_addr >> 1) & 0x1;
    memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
    ASSERT_EQ(0x10a0, m_data16[index_16]);
}

TEST_F(Intc1xIOVFunctionTest, EIOVTest7){
    uint32_t channel = 0;
    uint32_t data = 0x1080;        //mask = 1, eirf = 1
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    uint8_t  rdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    intc1->HardReset();
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, 2);
    intc1->CyclicHandler();

    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);

    intc1->ReqIntByPeripheral(1);    //req int from channel 1
    intc1->CyclicHandler();

    intc1->TargetRead(sideband, eic_addr + channel*2, rdata, 2);
    uint32_t index_16 = (eic_addr >> 1) & 0x1;
    memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
    ASSERT_EQ(0x1080, m_data16[index_16]);
}


TEST_F(Intc1xIOVFunctionTest, FIOVTest1){
    uint32_t channel = 0;
    uint32_t data = 0x1000;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    uint8_t  rdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    intc1->HardReset();
    sideband.SetDM(1);

    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata, 2);
    intc1->CyclicHandler();

    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);

    intc1->TargetRead(sideband, fic_addr + channel*2, rdata, 2);
    uint32_t index_16 = (fic_addr >> 1) & 0x1;
    memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
    ASSERT_EQ(0x1000, m_data16[index_16]);
}

TEST_F(Intc1xIOVFunctionTest, FIOVTest2){
    uint32_t channel = 0;
    uint32_t data = 0x1000;
    uint32_t dbmk_data = 0x7;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    uint8_t *dbmk_wdata = reinterpret_cast<uint8_t *>(&dbmk_data);
    uint8_t  rdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    intc1->HardReset();
    sideband.SetDM(1);
    intc1->TargetWrite(sideband, dbmk_addr, dbmk_wdata, 2);
    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata, 2);
    intc1->CyclicHandler();

    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);

    intc1->ReqIntByFeint(0);
    intc1->CyclicHandler();

    intc1->TargetRead(sideband, fic_addr + channel*2, rdata, 2);
    uint32_t index_16 = (fic_addr >> 1) & 0x1;
    memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
    ASSERT_EQ(0x1020, m_data16[index_16]);
}

TEST_F(Intc1xIOVFunctionTest, FIOVTest3){
    uint32_t channel = 0;
    uint32_t data = 0x1000;
    uint32_t dbmk_data = 0x7;
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    uint8_t *dbmk_wdata = reinterpret_cast<uint8_t *>(&dbmk_data);
    uint8_t  rdata[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    intc1->HardReset();
    sideband.SetDM(1);
    intc1->TargetWrite(sideband, dbmk_addr, dbmk_wdata, 2);
    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata, 2);
    intc1->CyclicHandler();

    bool flag = intc1->m_intc1_queue.empty();
    ASSERT_EQ(true, !flag);

    intc1->ReqIntByFeint(1);        //req int from channel 1
    intc1->CyclicHandler();

    intc1->TargetRead(sideband, fic_addr + channel*2, rdata, 2);
    uint32_t index_16 = (fic_addr >> 1) & 0x1;
    memcpy (&m_data16[index_16], rdata, sizeof(m_data16));
    ASSERT_EQ(0x1000, m_data16[index_16]);
}

class Intc1_UpdateHighPriorityFromCpuMode_Test
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<PhysAddr, int32_t, ByteSize, ByteSize, int32_t >> {
protected:
	Intc1_UpdateHighPriorityFromCpuMode_Test() {}
  virtual ~Intc1_UpdateHighPriorityFromCpuMode_Test() {}
  virtual void SetUp() {
    mock_fsbus = new MockFsBus ();
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_20, (ArchClass)E3V5_S);
    intc1 = new CIntc1(mock_pe, pe_id, mock_fsbus, (CoreType)CORE_G4MH, (CoreVersion)0x20);
    intc1->SetIHVEFromIHVCFG(1);

    EXPECT_CALL((*mock_fsbus), RemoveCyclicHandler (intc1)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus), AddCyclicHandler (intc1)).Times(AtLeast(0));
  }
  virtual void TearDown() {
      delete mock_fsbus;
      delete mock_pe;
      delete intc1;
  }

public:
  MockFsBus* mock_fsbus;
  SideBand sideband;
  MockProcElement *mock_pe;
  CIntc1 *intc1;
  uint32_t pe_id = 0;
  uint32_t eeic_addr = 0xFFFC0200;
  uint32_t eic_addr = 0xFFFC0000;
  uint32_t eibd_addr = 0xFFFC0100;
  uint32_t fibd_addr = 0xFFFC01C0;
  uint32_t eibg_addr = 0xFFFC0280;
  uint32_t fibg_addr = 0xFFFC02C0;
  uint32_t fnc_addr = 0xFFFC00A0;
  uint32_t fic_addr = 0xFFFC00C0;
  uint32_t dbmk_addr = 0xFFFC0080;

  union {
        uint32_t m_data32;
        uint16_t m_data16[2];
        uint8_t  m_data8[4];
  };

private:
};

TEST_F( Intc1_UpdateHighPriorityFromCpuMode_Test, BGEIINT_canceled_in_cpu_host){
    uint32_t channel = 0;
    uint32_t data = 0x1000;        //eirf = 1
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    intc1->m_is_cpu_gm = 0;
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, SIZE_16BIT);

    uint32_t data1 = 0x8100;        //GM=1, GPID=1
    uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&data1);
    intc1->TargetWrite(sideband, eibd_addr + channel*4, wdata1, SIZE_32BIT);

    uint32_t data2 = 0x5;        //
    uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&data2);
    intc1->TargetWrite(sideband, eibg_addr , wdata2, SIZE_32BIT);

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(3);
    intc1->m_is_cpu_gm = 1;
    intc1->CyclicHandler();

    intc1->m_is_cpu_gm = 0;
    intc1->CyclicHandler();

}

TEST_F( Intc1_UpdateHighPriorityFromCpuMode_Test, GMEIINT_canceled_in_cpu_host){
    uint32_t channel = 0;
    uint32_t data = 0x1000;        //eirf = 1
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    intc1->m_is_cpu_gm = 0;
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, SIZE_16BIT);

    uint32_t data1 = 0x8000;        //GM=1, GPID=0
    uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&data1);
    intc1->TargetWrite(sideband, eibd_addr + channel*4, wdata1, SIZE_32BIT);

    uint32_t data2 = 0x5;        //
    uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&data2);
    intc1->TargetWrite(sideband, eibg_addr , wdata2, SIZE_32BIT);

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(3);
    intc1->m_is_cpu_gm = 1;
    intc1->CyclicHandler();

    intc1->m_is_cpu_gm = 0;
    intc1->CyclicHandler();

}

TEST_F( Intc1_UpdateHighPriorityFromCpuMode_Test, BGFEINT_canceled_in_cpu_host){
    uint32_t channel = 0;
    uint32_t data = 0x1000;        //eirf = 1
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    sideband.SetDM(1);
    intc1->m_is_cpu_gm = 0;
    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata, SIZE_16BIT);

    uint32_t data1 = 0x8100;        //GM=1, GPID=1
    uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&data1);
    intc1->TargetWrite(sideband, fibd_addr + channel*4, wdata1, SIZE_32BIT);

    uint32_t data2 = 0x1;        //
    uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&data2);
    intc1->TargetWrite(sideband, fibg_addr , wdata2, SIZE_32BIT);

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(3);
    intc1->m_is_cpu_gm = 1;
    intc1->CyclicHandler();

    intc1->m_is_cpu_gm = 0;
    intc1->CyclicHandler();

}

TEST_F( Intc1_UpdateHighPriorityFromCpuMode_Test, GMFEINT_canceled_in_cpu_host){
    uint32_t channel = 0;
    uint32_t data = 0x1000;        //eirf = 1
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    sideband.SetDM(1);
    intc1->m_is_cpu_gm = 0;
    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata, SIZE_16BIT);

    uint32_t data1 = 0x8000;        //GM=1, GPID=0
    uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&data1);
    intc1->TargetWrite(sideband, fibd_addr + channel*4, wdata1, SIZE_32BIT);

    uint32_t data2 = 0x1;        //
    uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&data2);
    intc1->TargetWrite(sideband, fibg_addr , wdata2, SIZE_32BIT);

    EXPECT_CALL((*mock_pe), GetHtPtr()).Times(3);
    intc1->m_is_cpu_gm = 1;
    intc1->CyclicHandler();

    intc1->m_is_cpu_gm = 0;
    intc1->CyclicHandler();
}

TEST_F( Intc1_UpdateHighPriorityFromCpuMode_Test, BGEIINT_GMEIINT_same_pri_1){

    uint32_t data = 0x1003;        //eirf = 1
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();

    //setting for BGEIINT channel 0
    uint32_t channel = 0;
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, SIZE_16BIT);

    uint32_t data1 = 0x8100;        //GM=1, GPID=1
    uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&data1);
    intc1->TargetWrite(sideband, eibd_addr + channel*4, wdata1, SIZE_32BIT);

    uint32_t data2 = 0x5;        //
    uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&data2);
    intc1->TargetWrite(sideband, eibg_addr , wdata2, SIZE_32BIT);

    intc1->m_is_cpu_gm = 1;
    intc1->UpdateHighPriorityFromCpuMode(-1);

    //setting for GMEIINT channel 1
    channel = 1;
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, SIZE_16BIT);

    data1 = 0x8000;        //GM=1, GPID=0
    uint8_t *wdata3 = reinterpret_cast<uint8_t *>(&data1);
    intc1->TargetWrite(sideband, eibd_addr + channel*4, wdata3, SIZE_32BIT);

    intc1->m_is_cpu_gm = 1;
    intc1->UpdateHighPriorityFromCpuMode(-1);

    ASSERT_EQ(0x0U, intc1->m_intc1_priority[-1]->m_channel_eiint);
    ASSERT_EQ(0x3, intc1->m_intc1_priority[-1]->m_priority_eiint);
}

TEST_F( Intc1_UpdateHighPriorityFromCpuMode_Test, BGEIINT_GMEIINT_same_pri_2){

    uint32_t data = 0x1003;        //eirf = 1
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();

    //setting for BGEIINT channel 0
    uint32_t channel = 3;
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, SIZE_16BIT);

    uint32_t data1 = 0x8100;        //GM=1, GPID=1
    uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&data1);
    intc1->TargetWrite(sideband, eibd_addr + channel*4, wdata1, SIZE_32BIT);

    uint32_t data2 = 0x5;        //
    uint8_t *wdata2 = reinterpret_cast<uint8_t *>(&data2);
    intc1->TargetWrite(sideband, eibg_addr , wdata2, SIZE_32BIT);

    intc1->m_is_cpu_gm = 1;
    intc1->UpdateHighPriorityFromCpuMode(-1);

    //setting for GMEIINT channel 1
    channel = 1;
    intc1->TargetWrite(sideband, eic_addr + channel*2, wdata, SIZE_16BIT);

    data1 = 0x8000;        //GM=1, GPID=0
    uint8_t *wdata3 = reinterpret_cast<uint8_t *>(&data1);
    intc1->TargetWrite(sideband, eibd_addr + channel*4, wdata3, SIZE_32BIT);

    intc1->m_is_cpu_gm = 1;
    intc1->UpdateHighPriorityFromCpuMode(-1);

    ASSERT_EQ(0x1U, intc1->m_intc1_priority[-1]->m_channel_eiint);
    ASSERT_EQ(0x3, intc1->m_intc1_priority[-1]->m_priority_eiint);
}

TEST_F( Intc1_UpdateHighPriorityFromCpuMode_Test, BGFEINT_GMFEINT_same_pri_1){

    uint32_t data = 0x1000;        //eirf = 1
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    sideband.SetDM(1);
    intc1->m_is_cpu_gm = 0;

    //setting for GMFEINT channel 0
    uint32_t channel = 0;
    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata, SIZE_16BIT);

    uint32_t data1 = 0x8000;        //GM=1, GPID=0
    uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&data1);
    intc1->TargetWrite(sideband, fibd_addr + channel*4, wdata1, SIZE_32BIT);

    intc1->m_is_cpu_gm = 1;
    intc1->UpdateHighPriorityFromCpuMode(-1);

    //setting for BGFEINT channel 1
    channel = 1;
    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata, SIZE_16BIT);

    data1 = 0x8100;        //GM=1, GPID=1
    uint8_t *wdata3 = reinterpret_cast<uint8_t *>(&data1);
    intc1->TargetWrite(sideband, fibd_addr + channel*4, wdata3, SIZE_32BIT);

    uint32_t data4 = 0x1;        //
    uint8_t *wdata4 = reinterpret_cast<uint8_t *>(&data4);
    intc1->TargetWrite(sideband, fibg_addr , wdata4, SIZE_32BIT);

    intc1->m_is_cpu_gm = 1;
    intc1->UpdateHighPriorityFromCpuMode(-1);

    ASSERT_EQ(0x0U, (intc1->m_intc1_priority[-1]->m_channel_feint) - CIntc1::FEINT_CHANNEL );

}

TEST_F( Intc1_UpdateHighPriorityFromCpuMode_Test, BGFEINT_GMFEINT_same_pri_2){

    uint32_t data = 0x1000;        //eirf = 1
    uint8_t *wdata = reinterpret_cast<uint8_t *>(&data);
    intc1->HardReset();
    sideband.SetDM(1);
    intc1->m_is_cpu_gm = 0;

    //setting for GMFEINT channel 1
    uint32_t channel = 1;
    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata, SIZE_16BIT);

    uint32_t data1 = 0x8000;        //GM=1, GPID=0
    uint8_t *wdata1 = reinterpret_cast<uint8_t *>(&data1);
    intc1->TargetWrite(sideband, fibd_addr + channel*4, wdata1, SIZE_32BIT);

    intc1->m_is_cpu_gm = 1;
    intc1->UpdateHighPriorityFromCpuMode(-1);

    //setting for BGFEINT channel 0
    channel = 0;
    intc1->TargetWrite(sideband, fic_addr + channel*2, wdata, SIZE_16BIT);

    data1 = 0x8100;        //GM=1, GPID=1
    uint8_t *wdata3 = reinterpret_cast<uint8_t *>(&data1);
    intc1->TargetWrite(sideband, fibd_addr + channel*4, wdata3, SIZE_32BIT);

    uint32_t data4 = 0x1;        //
    uint8_t *wdata4 = reinterpret_cast<uint8_t *>(&data4);
    intc1->TargetWrite(sideband, fibg_addr , wdata4, SIZE_32BIT);

    intc1->m_is_cpu_gm = 1;
    intc1->UpdateHighPriorityFromCpuMode(-1);

    ASSERT_EQ(0x0U, (intc1->m_intc1_priority[-1]->m_channel_feint) - CIntc1::FEINT_CHANNEL );

}


int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
