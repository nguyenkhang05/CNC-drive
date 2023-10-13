/**
 * @file test_bar_cbar.cpp
 *
 * @date Created on: 2016
 * @author Author: Yvo <y.vo.wg@rvc.renesas.com>
 * @brief UNIT TEST for TRegBar. (SAMPLE)
 * @par Copyright: 2016 Renesas Electronics Corp.
 */

#include <memory>
#include <map>
#include <stdexcept>
#include "bar.h"
#include "mock_sideband.h"
#include "gmock/gmock.h"
#include "mock_fsbus.h"

using ::testing::AtLeast;
using ::testing::_;
using ::testing::Return;

class BarRegAccessTestINIT
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<int32_t, ByteSize, ByteSize>> {
protected:
  BarRegAccessTestINIT() {}
  virtual ~BarRegAccessTestINIT() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    mock_fsbus = new MockFsBus();
    bar = new CBar(&m_pes, mock_fsbus,BAR_STANDARD);

    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBitPos()).Times(0);
    EXPECT_CALL(sideband, GetBitOp()).Times(0);
    EXPECT_CALL(sideband, GetSecure()).Times(0);
    EXPECT_CALL(sideband, GetDM()).Times(0);
    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBMID()).Times(0);
    EXPECT_CALL(sideband, GetSPID()).Times(0);
    EXPECT_CALL(sideband, GetValue()).Times(0);

    EXPECT_CALL((*mock_fsbus),RemoveCyclicHandler (bar)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus),AddCyclicHandler (bar)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete bar;
                            delete mock_fsbus;}

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *mock_fsbus;
  PeVec m_pes;
  CBar *bar;
  uint32_t pe_id = 0;

private:
};

/*!
 * @brief Check BAR register read/write operation.
 */
TEST_P(BarRegAccessTestINIT, CheckBarRegReadWrite_INIT) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  {
    bar->HardReset();
    for (uint32_t  channel= 0 ; channel < 16; channel++){
        PhysAddr addr = TRegBar::BRINIT_OFFSET + channel * 0x10;
          {
            SCOPED_TRACE("Write");
            if (size == expect_size) {
              ASSERT_NO_THROW(bar->TargetWrite(sideband, addr, wdata, size));
            } else {
              // Expect throw if "Un-guaranteed Size Access".
              ASSERT_THROW(bar->TargetWrite(sideband, addr, wdata, size), std::range_error);
            }
          }
          {
            SCOPED_TRACE("Read");
            if (size == expect_size) {
              ASSERT_NO_THROW(bar->TargetRead(sideband, addr, rdata, size));
              for (auto i = 0; i < size; i++) {
                ASSERT_EQ(0x0, rdata[i]);
              }
            } else {
              // Expect throw if "Un-guaranteed Size Access".
              ASSERT_THROW(bar->TargetWrite(sideband, addr, wdata, size), std::range_error);
            }
             }
       }
    }
}

std::tuple<int32_t, ByteSize, ByteSize> const RW_ParametersTable_INIT[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0x0, SIZE_8BIT, SIZE_8BIT),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersINIT, BarRegAccessTestINIT,
                        ::testing::ValuesIn(RW_ParametersTable_INIT));

class BarRegAccessTestEN
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<int32_t, ByteSize, ByteSize>> {
protected:
  BarRegAccessTestEN() {}
  virtual ~BarRegAccessTestEN() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    mock_fsbus = new MockFsBus();
    bar = new CBar(&m_pes, mock_fsbus,BAR_STANDARD);

    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBitPos()).Times(0);
    EXPECT_CALL(sideband, GetBitOp()).Times(0);
    EXPECT_CALL(sideband, GetSecure()).Times(0);
    EXPECT_CALL(sideband, GetDM()).Times(0);
    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBMID()).Times(0);
    EXPECT_CALL(sideband, GetSPID()).Times(0);
    EXPECT_CALL(sideband, GetValue()).Times(0);

    EXPECT_CALL((*mock_fsbus),RemoveCyclicHandler (bar)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus),AddCyclicHandler (bar)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete bar;
                            delete mock_fsbus;}

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *mock_fsbus;
  PeVec m_pes;
  CBar *bar;
  uint32_t pe_id = 0;

private:
};

TEST_P(BarRegAccessTestEN, CheckBarRegReadWrite_EN) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  {
    bar->HardReset();
    for (uint32_t  channel= 0 ; channel < 16; channel++){
        PhysAddr addr = TRegBar::BREN_OFFSET + channel * 0x10;
          {
            SCOPED_TRACE("Write");
            if (size == expect_size) {
              ASSERT_NO_THROW(bar->TargetWrite(sideband, addr, wdata, size));
            } else {
              // Expect throw if "Un-guaranteed Size Access".
              ASSERT_THROW(bar->TargetWrite(sideband, addr, wdata, size), std::range_error);
            }
          }
          {
            SCOPED_TRACE("Read");
            if (size == expect_size) {
              ASSERT_NO_THROW(bar->TargetRead(sideband, addr, rdata, size));
              for (auto i = 0; i < size; i++) {
                ASSERT_EQ(wdata[i], rdata[i]);
              }
            } else {
              // Expect throw if "Un-guaranteed Size Access".
              ASSERT_THROW(bar->TargetWrite(sideband, addr, wdata, size), std::range_error);
            }
             }
       }
    }
}

std::tuple<int32_t, ByteSize, ByteSize> const RW_ParametersTable_EN[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0x0, SIZE_8BIT, SIZE_8BIT),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersEN, BarRegAccessTestEN,
                        ::testing::ValuesIn(RW_ParametersTable_EN));

class BarRegAccessTestCHK
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<int32_t, ByteSize, ByteSize>> {
protected:
  BarRegAccessTestCHK() {}
  virtual ~BarRegAccessTestCHK() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    mock_fsbus = new MockFsBus();
    bar = new CBar(&m_pes, mock_fsbus,BAR_STANDARD);

    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBitPos()).Times(0);
    EXPECT_CALL(sideband, GetBitOp()).Times(0);
    EXPECT_CALL(sideband, GetSecure()).Times(0);
    EXPECT_CALL(sideband, GetDM()).Times(0);
    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBMID()).Times(0);
    EXPECT_CALL(sideband, GetSPID()).Times(0);
    EXPECT_CALL(sideband, GetValue()).Times(0);

    EXPECT_CALL((*mock_fsbus),RemoveCyclicHandler (bar)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus),AddCyclicHandler (bar)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete bar;
                            delete mock_fsbus;}

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *mock_fsbus;
  PeVec m_pes;
  CBar *bar;
  uint32_t pe_id = 0;

private:
};

TEST_P(BarRegAccessTestCHK, CheckBarRegReadWrite_CHK) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  {
    bar->HardReset();
    for (uint32_t  bar_id= 0 ; bar_id < 8; bar_id++){
        for (uint32_t  channel= 0 ; channel < 16; channel++){
        PhysAddr addr = TRegBar::BRCHK_OFFSET + channel * 0x10 + bar_id * 0x100;
          {
            SCOPED_TRACE("Write");
            if (size == expect_size) {
              ASSERT_NO_THROW(bar->TargetWrite(sideband, addr, wdata, size));
            } else {
              // Expect throw if "Un-guaranteed Size Access".
              ASSERT_THROW(bar->TargetWrite(sideband, addr, wdata, size), std::range_error);
            }
          }
          {
            SCOPED_TRACE("Read");
            if (size == expect_size) {
              ASSERT_NO_THROW(bar->TargetRead(sideband, addr, rdata, size));
              for (auto i = 0; i < size; i++) {
                ASSERT_EQ((0x0), rdata[i]);
              }
            } else {
              // Expect throw if "Un-guaranteed Size Access".
              ASSERT_THROW(bar->TargetWrite(sideband, addr, wdata, size), std::range_error);
            }
             }
       }
    }
  }
}

std::tuple<int32_t, ByteSize, ByteSize> const RW_ParametersTable_CHK[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0x0, SIZE_8BIT, SIZE_8BIT),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersCHK, BarRegAccessTestCHK,
                        ::testing::ValuesIn(RW_ParametersTable_CHK));

class BarRegAccessTestSYN
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<int32_t, ByteSize, ByteSize, int32_t>> {
protected:
  BarRegAccessTestSYN() {}
  virtual ~BarRegAccessTestSYN() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    mock_fsbus = new MockFsBus();
    bar = new CBar(&m_pes, mock_fsbus,BAR_STANDARD);

    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBitPos()).Times(0);
    EXPECT_CALL(sideband, GetBitOp()).Times(0);
    EXPECT_CALL(sideband, GetSecure()).Times(0);
    EXPECT_CALL(sideband, GetDM()).Times(0);
    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBMID()).Times(0);
    EXPECT_CALL(sideband, GetSPID()).Times(0);
    EXPECT_CALL(sideband, GetValue()).Times(0);

    EXPECT_CALL((*mock_fsbus),RemoveCyclicHandler (bar)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus),AddCyclicHandler (bar)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete bar;
                            delete mock_fsbus;}

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *mock_fsbus;
  PeVec m_pes;
  CBar *bar;
  uint32_t pe_id = 0;

private:
};

TEST_P(BarRegAccessTestSYN, CheckBarRegReadWrite_SYN) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  int32_t expect_data = std::get<3>(GetParam());
  {
    bar->HardReset();
    for (uint32_t  bar_id= 0 ; bar_id < 8; bar_id++){
        for (uint32_t  channel= 0 ; channel < 16; channel++){
        PhysAddr addr = TRegBar::BRSYN_OFFSET + channel * 0x10 + bar_id * 0x100;
          {
            SCOPED_TRACE("Write");
            if (size == expect_size) {
              ASSERT_NO_THROW(bar->TargetWrite(sideband, addr, wdata, size));
            } else {
              // Expect throw if "Un-guaranteed Size Access".
              ASSERT_THROW(bar->TargetWrite(sideband, addr, wdata, size), std::range_error);
            }
          }
          {
            SCOPED_TRACE("Read");
            if (size == expect_size) {
              ASSERT_NO_THROW(bar->TargetRead(sideband, addr, rdata, size));
              for (auto i = 0; i < size; i++) {
                ASSERT_EQ((expect_data), rdata[i]);
              }
            } else {
              // Expect throw if "Un-guaranteed Size Access".
              ASSERT_THROW(bar->TargetWrite(sideband, addr, wdata, size), std::range_error);
            }
             }
       }
    }
  }
}

std::tuple<int32_t, ByteSize, ByteSize, int32_t> const RW_ParametersTable_SYN[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT,1),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT,1),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT,1),
  /* 0002 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT,1),
  /* 0002 */ std::make_tuple(0x0, SIZE_8BIT, SIZE_8BIT,0),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersSYN, BarRegAccessTestSYN,
                        ::testing::ValuesIn(RW_ParametersTable_SYN));


class BarRegAccessTestINITDB
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<int32_t, ByteSize, ByteSize>> {
protected:
  BarRegAccessTestINITDB() {}
  virtual ~BarRegAccessTestINITDB() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    mock_fsbus = new MockFsBus();
    bar = new CBar(&m_pes, mock_fsbus,BAR_STANDARD);

    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBitPos()).Times(0);
    EXPECT_CALL(sideband, GetBitOp()).Times(0);
    EXPECT_CALL(sideband, GetSecure()).Times(0);
    EXPECT_CALL(sideband, GetDM()).Times(0);
    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBMID()).Times(0);
    EXPECT_CALL(sideband, GetSPID()).Times(0);
    EXPECT_CALL(sideband, GetValue()).Times(0);

    EXPECT_CALL((*mock_fsbus),RemoveCyclicHandler (bar)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus),AddCyclicHandler (bar)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete bar;
                            delete mock_fsbus;}

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *mock_fsbus;
  PeVec m_pes;
  CBar *bar;
  uint32_t pe_id = 0;

private:
};

/*!
 * @brief Check BAR register read/write operation.
 */
TEST_P(BarRegAccessTestINITDB, CheckBarRegReadWrite_INIT) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  {
    bar->HardReset();
    for (uint32_t  channel= 0 ; channel < 16; channel++){
        PhysAddr addr = TRegBar::BRINIT_OFFSET + channel * 0x10;
          {
            SCOPED_TRACE("Write");
            if (size == expect_size) {
              ASSERT_NO_THROW(bar->TargetWriteDebug(addr, wdata, size));
            } else {
              // Expect throw if "Un-guaranteed Size Access".
              ASSERT_THROW(bar->TargetWriteDebug(addr, wdata, size), std::range_error);
            }
          }
          {
            SCOPED_TRACE("Read");
            if (size == expect_size) {
              ASSERT_NO_THROW(bar->TargetReadDebug(addr, rdata, size));
              for (auto i = 0; i < size; i++) {
                ASSERT_EQ(0x0, rdata[i]);
              }
            } else {
              // Expect throw if "Un-guaranteed Size Access".
              ASSERT_THROW(bar->TargetWriteDebug(addr, wdata, size), std::range_error);
            }
             }
       }
    }
}

std::tuple<int32_t, ByteSize, ByteSize> const RW_ParametersTable_INITDB[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0x0, SIZE_8BIT, SIZE_8BIT),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersINITDB, BarRegAccessTestINITDB,
                        ::testing::ValuesIn(RW_ParametersTable_INITDB));

class BarRegAccessTestENDB
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<int32_t, ByteSize, ByteSize>> {
protected:
  BarRegAccessTestENDB() {}
  virtual ~BarRegAccessTestENDB() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    mock_fsbus = new MockFsBus();
    bar = new CBar(&m_pes, mock_fsbus,BAR_STANDARD);

    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBitPos()).Times(0);
    EXPECT_CALL(sideband, GetBitOp()).Times(0);
    EXPECT_CALL(sideband, GetSecure()).Times(0);
    EXPECT_CALL(sideband, GetDM()).Times(0);
    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBMID()).Times(0);
    EXPECT_CALL(sideband, GetSPID()).Times(0);
    EXPECT_CALL(sideband, GetValue()).Times(0);

    EXPECT_CALL((*mock_fsbus),RemoveCyclicHandler (bar)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus),AddCyclicHandler (bar)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete bar;
                            delete mock_fsbus;}

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *mock_fsbus;
  PeVec m_pes;
  CBar *bar;
  uint32_t pe_id = 0;

private:
};

TEST_P(BarRegAccessTestENDB, CheckBarRegReadWrite_EN) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  {
    bar->HardReset();
    for (uint32_t  channel= 0 ; channel < 16; channel++){
        PhysAddr addr = TRegBar::BREN_OFFSET + channel * 0x10;
          {
            SCOPED_TRACE("Write");
            if (size == expect_size) {
              ASSERT_NO_THROW(bar->TargetWriteDebug(addr, wdata, size));
            } else {
              // Expect throw if "Un-guaranteed Size Access".
              ASSERT_THROW(bar->TargetWriteDebug(addr, wdata, size), std::range_error);
            }
          }
          {
            SCOPED_TRACE("Read");
            if (size == expect_size) {
              ASSERT_NO_THROW(bar->TargetReadDebug(addr, rdata, size));
              for (auto i = 0; i < size; i++) {
                ASSERT_EQ(wdata[i], rdata[i]);
              }
            } else {
              // Expect throw if "Un-guaranteed Size Access".
              ASSERT_THROW(bar->TargetWriteDebug(addr, wdata, size), std::range_error);
            }
             }
       }
    }
}

std::tuple<int32_t, ByteSize, ByteSize> const RW_ParametersTable_ENDB[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0x0, SIZE_8BIT, SIZE_8BIT),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersENDB, BarRegAccessTestENDB,
                        ::testing::ValuesIn(RW_ParametersTable_ENDB));

class BarRegAccessTestCHKDB
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<int32_t, ByteSize, ByteSize>> {
protected:
  BarRegAccessTestCHKDB() {}
  virtual ~BarRegAccessTestCHKDB() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    mock_fsbus = new MockFsBus();
    bar = new CBar(&m_pes, mock_fsbus,BAR_STANDARD);

    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBitPos()).Times(0);
    EXPECT_CALL(sideband, GetBitOp()).Times(0);
    EXPECT_CALL(sideband, GetSecure()).Times(0);
    EXPECT_CALL(sideband, GetDM()).Times(0);
    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBMID()).Times(0);
    EXPECT_CALL(sideband, GetSPID()).Times(0);
    EXPECT_CALL(sideband, GetValue()).Times(0);

    EXPECT_CALL((*mock_fsbus),RemoveCyclicHandler (bar)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus),AddCyclicHandler (bar)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete bar;
                            delete mock_fsbus;}

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *mock_fsbus;
  PeVec m_pes;
  CBar *bar;
  uint32_t pe_id = 0;

private:
};

TEST_P(BarRegAccessTestCHKDB, CheckBarRegReadWrite_CHK) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  {
    bar->HardReset();
    for (uint32_t  bar_id= 0 ; bar_id < 8; bar_id++){
        for (uint32_t  channel= 0 ; channel < 16; channel++){
        PhysAddr addr = TRegBar::BRCHK_OFFSET + channel * 0x10 + bar_id * 0x100;
          {
            SCOPED_TRACE("Write");
            if (size == expect_size) {
              ASSERT_NO_THROW(bar->TargetWrite(sideband, addr, wdata, size));
            } else {
              // Expect throw if "Un-guaranteed Size Access".
              ASSERT_THROW(bar->TargetWrite(sideband, addr, wdata, size), std::range_error);
            }
          }
          {
            SCOPED_TRACE("Read");
            if (size == expect_size) {
              ASSERT_NO_THROW(bar->TargetRead(sideband, addr, rdata, size));
              for (auto i = 0; i < size; i++) {
                ASSERT_EQ((0x0), rdata[i]);
              }
            } else {
              // Expect throw if "Un-guaranteed Size Access".
              ASSERT_THROW(bar->TargetWrite(sideband, addr, wdata, size), std::range_error);
            }
             }
       }
    }
  }
}

std::tuple<int32_t, ByteSize, ByteSize> const RW_ParametersTable_CHKDB[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT),
  /* 0002 */ std::make_tuple(0x0, SIZE_8BIT, SIZE_8BIT),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersCHKDB, BarRegAccessTestCHKDB,
                        ::testing::ValuesIn(RW_ParametersTable_CHKDB));

class BarRegAccessTestSYNDB
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<int32_t, ByteSize, ByteSize, int32_t>> {
protected:
  BarRegAccessTestSYNDB() {}
  virtual ~BarRegAccessTestSYNDB() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    mock_fsbus = new MockFsBus();
    bar = new CBar(&m_pes, mock_fsbus,BAR_STANDARD);

    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBitPos()).Times(0);
    EXPECT_CALL(sideband, GetBitOp()).Times(0);
    EXPECT_CALL(sideband, GetSecure()).Times(0);
    EXPECT_CALL(sideband, GetDM()).Times(0);
    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBMID()).Times(0);
    EXPECT_CALL(sideband, GetSPID()).Times(0);
    EXPECT_CALL(sideband, GetValue()).Times(0);

    EXPECT_CALL((*mock_fsbus),RemoveCyclicHandler (bar)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus),AddCyclicHandler (bar)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete bar;
                            delete mock_fsbus;}

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *mock_fsbus;
  PeVec m_pes;
  CBar *bar;
  uint32_t pe_id = 0;

private:
};

TEST_P(BarRegAccessTestSYNDB, CheckBarRegReadWrite_SYN) {
  int32_t temp = static_cast<int32_t>(std::get<0>(GetParam()));
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = std::get<1>(GetParam());
  ByteSize expect_size = std::get<2>(GetParam());
  int32_t expect_data = std::get<3>(GetParam());
  {
    bar->HardReset();
    for (uint32_t  bar_id= 0 ; bar_id < 8; bar_id++){
        for (uint32_t  channel= 0 ; channel < 16; channel++){
        PhysAddr addr = TRegBar::BRSYN_OFFSET + channel * 0x10 + bar_id * 0x100;
          {
            SCOPED_TRACE("Write");
            if (size == expect_size) {
              ASSERT_NO_THROW(bar->TargetWriteDebug(addr, wdata, size));
            } else {
              // Expect throw if "Un-guaranteed Size Access".
              ASSERT_THROW(bar->TargetWriteDebug(addr, wdata, size), std::range_error);
            }
          }
          {
            SCOPED_TRACE("Read");
            if (size == expect_size) {
              ASSERT_NO_THROW(bar->TargetReadDebug(addr, rdata, size));
              for (auto i = 0; i < size; i++) {
                ASSERT_EQ((expect_data), rdata[i]);
              }
            } else {
              // Expect throw if "Un-guaranteed Size Access".
              ASSERT_THROW(bar->TargetWriteDebug(addr, wdata, size), std::range_error);
            }
             }
       }
    }
  }
}

std::tuple<int32_t, ByteSize, ByteSize, int32_t> const RW_ParametersTable_SYNDB[] = {
  /* 0000 */ std::make_tuple(0xf4f3f2f1, SIZE_32BIT, SIZE_8BIT,1),
  /* 0001 */ std::make_tuple(0xf8f7f6f5, SIZE_32BIT, SIZE_8BIT,1),
  /* 0002 */ std::make_tuple(0xfcfbfaf9, SIZE_8BIT, SIZE_8BIT,1),
  /* 0002 */ std::make_tuple(0xffffffff, SIZE_8BIT, SIZE_8BIT,1),
  /* 0002 */ std::make_tuple(0x0, SIZE_8BIT, SIZE_8BIT,0),
  // please write more more more test case.
};
INSTANTIATE_TEST_CASE_P(TestWithParametersSYNDB, BarRegAccessTestSYNDB,
                        ::testing::ValuesIn(RW_ParametersTable_SYNDB));


TEST_F(BarRegAccessTestINIT, CheckResetBarReg_INIT) {
  PhysAddr addr = TRegBar::BRCHK_OFFSET + 0;
  int32_t temp = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
  uint8_t expect_value[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = 1;

  ASSERT_NO_THROW(bar->TargetWrite(sideband, addr, wdata, size));
  bar->HardReset();
  ASSERT_NO_THROW(bar->TargetRead(sideband, addr, rdata, size));
  for (auto i = 0; i < size; i++) {
    ASSERT_EQ(expect_value[i], rdata[i]);
  }
}

TEST_F(BarRegAccessTestEN, CheckResetBarReg_EN) {
    PhysAddr addr = TRegBar::BREN_OFFSET + 0;
  int32_t temp = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
  uint8_t expect_value[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = 1;

  ASSERT_NO_THROW(bar->TargetWrite(sideband, addr, wdata, size));
  bar->HardReset();
  ASSERT_NO_THROW(bar->TargetRead(sideband, addr, rdata, size));
  for (auto i = 0; i < size; i++) {
    ASSERT_EQ(expect_value[i], rdata[i]);
  }
}

TEST_F(BarRegAccessTestCHK, CheckResetBarReg_CHK) {
    PhysAddr addr = TRegBar::BRCHK_OFFSET + 0;
  int32_t temp = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 1, 1, 1, 1, 1, 1, 1, 1 };
  uint8_t expect_value[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = 1;

  ASSERT_NO_THROW(bar->TargetWrite(sideband, addr, wdata, size));
  bar->HardReset();
  ASSERT_NO_THROW(bar->TargetRead(sideband, addr, rdata, size));
  for (auto i = 0; i < size; i++) {
    ASSERT_EQ(expect_value[i], rdata[i]);
  }
}

TEST_F(BarRegAccessTestSYN, CheckResetBarReg_SYN) {
    PhysAddr addr = TRegBar::BRSYN_OFFSET + 0;
  int32_t temp = 0xffffffff;
  uint8_t *wdata = reinterpret_cast<uint8_t *>(&temp);
  uint8_t rdata[] = { 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA, 0xA };
  uint8_t expect_value[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
  ByteSize size = 1;

  ASSERT_NO_THROW(bar->TargetWrite(sideband, addr, wdata, size));
  ASSERT_NO_THROW(bar->TargetRead(sideband, addr, rdata, size));
  bar->HardReset();
  ASSERT_NO_THROW(bar->TargetRead(sideband, addr, rdata, size));
  for (auto i = 0; i < size; i++) {
    ASSERT_EQ(expect_value[i], rdata[i]);
  }
}

TEST_F(BarRegAccessTestCHK, CheckGetBRCHKTPtr) {
  bar->HardReset();
  for (uint32_t bar_id = 0; bar_id < 8; bar_id++){
      for (uint32_t channel =0; channel <16; channel++){
          PhysAddr addr = TRegBar::BRCHK_OFFSET + channel * 0x10 + bar_id * 0x100;
          uint8_t wdata[] = { 0x1A, 0, 0, 0, 0, 0, 0, 0 };
          ASSERT_NO_THROW(bar->TargetWrite(sideband, addr, wdata, SIZE_8BIT));
          TRegBRCHK* brchk = bar->GetBRCHKPtr(channel, bar_id);

          ASSERT_EQ(0x0U, brchk->GetData());
      }
  }
}

TEST_F(BarRegAccessTestSYN, CheckGetBRSYNTPtr) {
  bar->HardReset();
  for (uint32_t bar_id = 0; bar_id < 8; bar_id++){
      for (uint32_t channel =0; channel <16; channel++){
          PhysAddr addr = TRegBar::BRSYN_OFFSET + channel * 0x10 + bar_id * 0x100;
          uint8_t wdata[] = { 0x1A, 0, 0, 0, 0, 0, 0, 0 };
          ASSERT_NO_THROW(bar->TargetWrite(sideband, addr, wdata, SIZE_8BIT));
          TRegBRSYN* brsyn = bar->GetBRSYNPtr(channel, bar_id);

          ASSERT_EQ(0x0U, brsyn->GetData());
      }
  }
}

TEST_F(BarRegAccessTestINIT, CheckGetBRINITTPtr) {
  bar->HardReset();
  for (uint32_t channel =0; channel <16; channel++){
      PhysAddr addr = TRegBar::BRINIT_OFFSET + channel * 0x10;
      uint8_t wdata[] = { 0x1A, 0, 0, 0, 0, 0, 0, 0 };
      ASSERT_NO_THROW(bar->TargetWrite(sideband, addr, wdata, SIZE_8BIT));
      TRegBRINIT* brinit = bar->GetBRINITPtr(channel);

      ASSERT_EQ(0x0U, brinit->GetData());
  }
}

TEST_F(BarRegAccessTestEN, CheckGetBRENTPtr) {
  bar->HardReset();
  for (uint32_t channel =0; channel <16; channel++){
      PhysAddr addr = TRegBar::BREN_OFFSET + channel * 0x10;
      uint8_t wdata[] = { 0x1A, 0, 0, 0, 0, 0, 0, 0 };
      ASSERT_NO_THROW(bar->TargetWrite(sideband, addr, wdata, SIZE_8BIT));
      TRegBREN* bren = bar->GetBRENPtr(channel);

      ASSERT_EQ(0x1AU, bren->GetData());
  }
}

TEST_F(BarRegAccessTestINIT, CheckSetGetINIT) {
  bar->HardReset();
  for (uint32_t channel =0; channel <16; channel++){
      bar->Set_BRINIT(channel,0x1A);
      ASSERT_EQ(0x0U, bar->Get_BRINIT(channel));
  }
}

TEST_F(BarRegAccessTestEN, CheckSetGetEN) {
  bar->HardReset();
  for (uint32_t channel =0; channel <16; channel++){
      bar->Set_BREN(channel,0x1A);
      ASSERT_EQ(0x1AU, bar->Get_BREN(channel));
  }
}
TEST_F(BarRegAccessTestCHK, CheckSetGetCHK) {
  bar->HardReset();
  for (uint32_t bar_id = 0; bar_id <8; bar_id++){
      for (uint32_t channel =0; channel <16; channel++){
          bar->Set_BRCHK(channel,bar_id,0x1A);
          ASSERT_EQ(0x0U, bar->Get_BRCHK(channel,bar_id));
      }
  }
}

TEST_F(BarRegAccessTestSYN, CheckSetGetSYN) {
  bar->HardReset();
  for (uint32_t bar_id = 0; bar_id <8; bar_id++){
      for (uint32_t channel =0; channel <16; channel++){
          bar->Set_BRSYN(channel,bar_id,0x1A);
          ASSERT_EQ(0x0U, bar->Get_BRSYN(channel,bar_id));
      }
  }
}


class BarTest_IsValidPeId : public testing::Test {
protected:
  BarTest_IsValidPeId() {}
  virtual ~BarTest_IsValidPeId() {}
  virtual void SetUp() {}
  virtual void TearDown() {}

public:
private:
};

TEST_F(BarTest_IsValidPeId, CheckIsValidPeIdFunc) {
  MockProcElement *mock_pe;
  MockFsBus *mock_fsbus;
  PeVec m_pes;
  CBar *bar;
  bool result;
  uint32_t pe_id = 0;
  // Case 1 PeVec.size == 0
  m_pes.clear();
  mock_fsbus = new MockFsBus();
  bar = new CBar(&m_pes,mock_fsbus, BAR_STANDARD);
  result = false;
  ASSERT_EQ(bar->IsValidPeId(pe_id), result);
  delete bar;
  delete mock_fsbus;
  m_pes.clear();

  // Case 2 PeVec.size == 1
  uint32_t init_id2[] = { 0 };
  std::map<uint32_t, bool> id2 = { { 0, true }, { 1, false }, { 2, false } }; // {pe_id, result}
  for (auto id : init_id2) {
    mock_pe = new MockProcElement(id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
  }
  mock_fsbus = new MockFsBus();
  bar = new CBar(&m_pes,mock_fsbus, BAR_STANDARD);
  for (auto id : id2) {
    ASSERT_EQ(bar->IsValidPeId(id.first), id.second);
  }
  delete bar;
  delete mock_fsbus;
  m_pes.clear();

  // Case 3 PeVec.size == 2
  uint32_t init_id3[] = { 0, 1 };
  std::map<uint32_t, bool> id3 = { { 0, true }, { 1, true }, { 2, false } }; // {pe_id, result}
  for (auto id : init_id3) {
    mock_pe = new MockProcElement(id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
  }
  mock_fsbus = new MockFsBus();
  bar = new CBar(&m_pes,mock_fsbus, BAR_STANDARD);
  for (auto id : id3) {
    ASSERT_EQ(bar->IsValidPeId(id.first), id.second);
  }
  delete bar;
  delete mock_fsbus;
  m_pes.clear();

  // Case 4 PeVec.size == 3
  uint32_t init_id4[] = { 0, 1, 2 };
  std::map<uint32_t, bool> id4 = { { 0, true }, { 1, true }, { 2, true }, {3, false} }; // {pe_id, result}
  for (auto id : init_id4) {
    mock_pe = new MockProcElement(id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
  }
  mock_fsbus = new MockFsBus();
  bar = new CBar(&m_pes,mock_fsbus, BAR_STANDARD);
  for (auto id : id4) {
    ASSERT_EQ(bar->IsValidPeId(id.first), id.second);
  }
  delete bar;
  delete mock_fsbus;
  m_pes.clear();

  // Case 5 PeVec.size == 4
  uint32_t init_id5[] = { 0, 1, 2, 3 };
  std::map<uint32_t, bool> id5 = { { 0, true }, { 1, true }, { 2, true }, {3, true}, {4, false}  }; // {pe_id, result}
  for (auto id : init_id5) {
    mock_pe = new MockProcElement(id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
  }
  mock_fsbus = new MockFsBus();
  bar = new CBar(&m_pes,mock_fsbus, BAR_STANDARD);
  for (auto id : id5) {
    ASSERT_EQ(bar->IsValidPeId(id.first), id.second);
  }
  delete bar;
  delete mock_fsbus;
  m_pes.clear();

  // Case 6 PeVec.size == 5
  uint32_t init_id6[] = { 0, 1, 2, 3, 4 };
  std::map<uint32_t, bool> id6 = { { 0, true }, { 1, true }, { 2, true }, {3, true}, {4, true}, {5, false}  }; // {pe_id, result}
  for (auto id : init_id6) {
    mock_pe = new MockProcElement(id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
  }
  mock_fsbus = new MockFsBus();
  bar = new CBar(&m_pes,mock_fsbus, BAR_STANDARD);
  for (auto id : id6) {
    ASSERT_EQ(bar->IsValidPeId(id.first), id.second);
  }
  delete bar;
  delete mock_fsbus;
  m_pes.clear();

  // Case 7 PeVec.size == 6
  uint32_t init_id7[] = { 0, 1, 2, 3, 4, 5 };
  std::map<uint32_t, bool> id7 = { { 0, true }, { 1, true }, { 2, true }, {3, true}, {4, true}, {5, true}, {6, false}  }; // {pe_id, result}
  for (auto id : init_id7) {
    mock_pe = new MockProcElement(id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
  }
  mock_fsbus = new MockFsBus();
  bar = new CBar(&m_pes,mock_fsbus, BAR_STANDARD);
  for (auto id : id7) {
    ASSERT_EQ(bar->IsValidPeId(id.first), id.second);
  }
  delete bar;
  delete mock_fsbus;
  m_pes.clear();

  // Case 8 PeVec.size == 7
  uint32_t init_id8[] = { 0, 1, 2, 3, 4, 5, 6 };
  std::map<uint32_t, bool> id8 = { { 0, true }, { 1, true }, { 2, true }, {3, true}, {4, true}, {5, true}, {6, true}, {7, false}}; // {pe_id, result}
  for (auto id : init_id8) {
    mock_pe = new MockProcElement(id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
  }
  mock_fsbus = new MockFsBus();
  bar = new CBar(&m_pes,mock_fsbus, BAR_STANDARD);
  for (auto id : id8) {
    ASSERT_EQ(bar->IsValidPeId(id.first), id.second);
  }
  delete bar;
  delete mock_fsbus;
  m_pes.clear();

  // Case 9 PeVec.size == 8
  uint32_t init_id9[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
  std::map<uint32_t, bool> id9 = { { 0, true }, { 1, true }, { 2, true }, {3, true}, {4, true}, {5, true}, {6, true}, {7, true}, {8, false}}; // {pe_id, result}
  for (auto id : init_id9) {
    mock_pe = new MockProcElement(id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
  }
  mock_fsbus = new MockFsBus();
  bar = new CBar(&m_pes,mock_fsbus, BAR_STANDARD);
  for (auto id : id9) {
    ASSERT_EQ(bar->IsValidPeId(id.first), id.second);
  }
  delete bar;
  delete mock_fsbus;
  m_pes.clear();
}

class ConvertSelfAddr
    : public testing::Test,
      public testing::WithParamInterface<std::tuple<int32_t, ByteSize, ByteSize>> {
protected:
	ConvertSelfAddr() {}
  virtual ~ConvertSelfAddr() {}
  virtual void SetUp() {
    mock_pe = new MockProcElement(pe_id, (CoreType)CORE_G4MH, (CoreVersion)CORE_VER_10, (ArchClass)E3V5_S);
    m_pes.push_back(mock_pe);
    mock_fsbus = new MockFsBus();
    bar = new CBar(&m_pes, mock_fsbus,BAR_STANDARD);

    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBitPos()).Times(0);
    EXPECT_CALL(sideband, GetBitOp()).Times(0);
    EXPECT_CALL(sideband, GetSecure()).Times(0);
    EXPECT_CALL(sideband, GetDM()).Times(0);
    EXPECT_CALL(sideband, GetUM()).Times(0);
    EXPECT_CALL(sideband, GetBMID()).Times(0);
    EXPECT_CALL(sideband, GetSPID()).Times(0);
    EXPECT_CALL(sideband, GetValue()).Times(0);

    EXPECT_CALL((*mock_fsbus),RemoveCyclicHandler (bar)).Times(AtLeast(0));
    EXPECT_CALL((*mock_fsbus),AddCyclicHandler (bar)).Times(AtLeast(0));
  }
  virtual void TearDown() { delete bar;
                            delete mock_fsbus;}

public:
  MockSideBand sideband;
  MockProcElement *mock_pe;
  MockFsBus *mock_fsbus;
  PeVec m_pes;
  CBar *bar;
  uint32_t pe_id = 0;


private:

};
//n: channel m: peid
//+0x0 + 0x10*n - BRnINIT
//+0x4 + 0x10*n - BRnEN
//+0x100 + 0x10*n - BRnCHKS
//+0x104 + 0x10*n - BRnSYNS
//+0x800 + 0x10*n + 0x100*m- BRnCHKm
//+0x804 + 0x10*n + 0x100*m- BRnSYNm

TEST_F(ConvertSelfAddr, ConvertSelfAddr_Regs) {
  bar->HardReset();
  for (uint32_t peid = 0; peid < 8; peid++ ){
    for (uint32_t channel =0; channel <16; channel++){
      //TranslateAddr(uint32_t peid, PhysAddr addr)
      ASSERT_EQ(0x10*channel, bar->ConvertSelfAddr(peid, 0x10*channel)); //BRnINIT
      ASSERT_EQ(4+ 0x10*channel, bar->ConvertSelfAddr(peid, 4+0x10*channel)); //BRnEN
      ASSERT_EQ(0x800 + 0x10*channel + 0x100*peid, bar->ConvertSelfAddr(peid, 0x100 +0x10*channel)); //BRnCHKS
      ASSERT_EQ(0x804 + 0x10*channel + 0x100*peid, bar->ConvertSelfAddr(peid, 0x104 +0x10*channel)); //BRnCHKS
      ASSERT_EQ(0x800 + 0x10*channel + 0x100*peid, bar->ConvertSelfAddr(peid, 0x800 + 0x10*channel + 0x100*peid)); //BRnCHKm
      ASSERT_EQ(0x804 + 0x10*channel + 0x100*peid, bar->ConvertSelfAddr(peid, 0x804 + 0x10*channel + 0x100*peid)); //BRnSYNm
    }
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleMock(&argc, argv);
  return RUN_ALL_TESTS();
}
